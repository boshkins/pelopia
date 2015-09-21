#Pelopia, an offline geocoding library
#High Level Design

##Overview

This document describes high level design for Pelias, a native, offline geocoding solution over a relatively small set of data (e.g. a city). 
The design is based on requirements specified in [doc/requirements.md](https://github.com/boshkins/pelopia/blob/master/doc/requirements.md)

The structure of the document is as follows:

- A list of API operations and supporting datatypes, in a notation close to C++
- Interfaces supporting API operations, with outlines of their naive (quick-and-dirty) implementations, as well as more size and/or speed efficient approaches.
- Pseudocode for the implementation of the API operations using the interfaces described earlier
- Examples of client code using the API
- Notes of testing, integration, multi-platform aspects etc
- A set of stories to act as the initial backlog for the project
- A list of implementation milestones  

##API operations

All externally visible C++ declarations of the API will be contained in the namespace Mapzen :: Pelopia. Any non-C++ wrappers will use prefix MzPel to avoid naming conflicts, if namespaces are not available.

### Supporting data types

	typede uint32_t ResultCount;

	typedef struct { 
		ResultCount maxResults; 
	} Format;
	const ResultCount AllResults = 0;
	const ResultCount DefaultResults = 10;

	typedef double Coordinate; // likely to change

	struct LatLon {
		Coordinate lat;
		Coordinate lon;
	};
 
	struct BoundingBox {
		LatLon topLeft;
		LatLon bottomRight;
	};
 
	typedef struct {
		enum { miles, kilometres, metres } units; 
		double distance; 
	} Distance;

	typedef string Category;

	typedef uint64_t Id;

	typedef double MatchQuality;

	class Response
	{	// a collection of results, sorted by descending quality 
	public:
		usigned int Count() const;

		bool Get ( unsigned int index, &Id id, &MatchQuality score ) const;
		Id Get ( unsigned int index ) const;
	}

### API methods

	class Dataset {

	public:
		Dataset ( const char* location ); // file or URL ?
 
		Response Search (const char* text, const LatLon& scope, Format format = { DefaultResults } );  
	
		Response Search (const char* text, const BoundingBox& scope, Format format = { DefaultResults });  
	
		Response Search (vector<string> Category, const LatLon& scope, Format format = { DefaultResults });
	  
		Response Search (vector<string> Category, const BoundingBox& scope, Format format = { DefaultResults });  
	
		Response Autocomplete (const char* text, const LatLon& scope);  
	
		Response Autocomplete (const char* text, const BoundingBox& scope);  
	
		Response Reverse ( const LatLon& point, const Distance& radius, Format format = { AllResults } );

		vector < GeoJSON::Feature > Place ( vector < Id > ids ); 

		vector < string > PlaceStringify ( vector < Id > ids );
 
	};

### GeoJSON objects

	namespace GeoJSON {
		// GeoJSON declarations, based on http://geojson.org/geojson-spec.html](http://geojson.org/geojson-spec.html

		class Geometry
		{	// for now, only Point in Geographic Coordinate Reference System 
		public:
			Coordinate Latitude () const;
			Coordinate Longitude () const;
		}

		class Feature
		{	// This is a GeoJSON Feature object corresponding to a single Location 
		public:

			const GeometryObject* Geometry() const;

			// Feature access, based on https://github.com/pelias/schema/blob/master/mappings/document.js

			const char* Layer () const;

			const char* Name () const;
			const char* Text () const; // same as Phrase in the schema ?

			const char* Address () const;
			const char* Alpha3 () const;

			const char* Admin0 () const;
			const char* Admin1 () const;
			const char* Admin1_abbr () const;
			const char* Admin2 () const;
			const char* LocalAdmin () const;

			const char* Locality () const;
			const char* Neighborhood () const;

			const Geometry& CenterPoint () const;

			// how to represent Shape ?

			unsigned int CategoryCount () const;
			const char* Category ( unsigned int index ) const; 
			// maybe add CategoryId to work with enum-like categories ?


			string Stringify () const;	
		};

	}


##Supporting interfaces and algorithms

It is important to note that read time containers are (at least conceptually) immutable, so they are best represented as a buffer loaded into memory without any additional processing. 
Write side can analyze the structure and choose alternative representations or suggest more efficient lookup methods.

The usefulness of naive implementations is in their simplicity, allowing for a rapid development of a functionally complete system. The quickly-built system then can be used to explore functionality issues, as well as investigated for performance bottlenecks. The feedback from performance study will drive replacement of naive designs with more performant ones. Care must be taken to insulate the rest of the systems from the details of naive/efficient designs, which is achieved through the use of interfaces.

Since the target language is C++, interfaces are modeled by abstract classes. 

###Data:
**Data**: a sequence of GeoJSON-derived objects. 
A naive implementation would use an STL vector of in-memory GeoJSON::Location objects.

	class Data {
	public:
		virtual const GeoJSON::Location& Get ( Id ) const = 0;
	};

A more memory-efficient representation would store serialized objects in a byte buffer (in memory or on disk) and involve an index:

**Index**: an array of offsets into data buffer; subscript is the object Id. Size of an Id value depends on the total size of the **Data**; 4 bytes should be enough but consider packing into less than 32 bits.   

The memory efficiency comes at the cost of reduced speed of access to individual objects, which can be addressed by adding a cache of inflated Location objects.

The byte buffer itself, if it resides on disk, can be split into blocks and cached.  


GeoJSON::Location objects returned by the Data interface do not have to be fully inflated. Since in many cases only selected properties of a given object will be of interest to the calling code, it may be beneficial to implement lazy access to properties. As another option, Location objects and/or their properties can be represented by references to corresponding portions of the data buffer. With that in mind, GeoJSON::Location should be fully abstract or at least opaque (using Pimpl pattern).

		class Feature
		{	// This is a GeoJSON Feature object corresponding to a single Location 
		public:
			const GeometryObject* Geometry() const = 0;
			// ... etc - see API Operations
		}

or, using the Pimpl idiom,

		class Feature
		{	// This is a GeoJSON Feature object corresponding to a single Location
		private:
			class Impl;	// defined in the implementation file 
			Impl* m_pimpl;
 
		public:
			const GeometryObject* Geometry() const;
			// ... etc - see API Operations 
		}

###Bounding Box search support
To support search filtered by a BoundingBox, we need an ability to filter locations by their center point. 

**Q: What about locations with shaped geometry? Intersection with the given BoundingBox requires something else** 

**LatIndex**: an array of pairs "coordinate, object Id" sorted by coordinate (latitude). There may be multiple entries with matching coordinates.
 
The principal operation will be to extract the subset of points located between 2 coordinates [*low* .. *high*]:

	FindSegment ( CoordIndex, LowCoord, HighCoord ) : ( firstIdx, lastIdx )
		firstIdx = FindFirstGreaterThanOrEqual ( CoordIndex, 0, size ( CoordIndex ), LowCoord )  
		lastIdx = FindLastLessThanOrEqual ( CoordIndex, firstIdx, size ( CoordIndex ), HighCoord )  
		return ( firstIdx, lastIdx )

The coordinate will be initially represented as a double. The alternatives would be float and integer-based fixed point; it may make sense to decrease precision of the coordinate to save space in this structure. The full-precision coordinates extracted from the objects will then be used at the scoring stage to filter out false hits, if necessary. 

For additional memory savings at the cost of lookup speed, pairs "coordinate, object Id" may be packed into fewer than 12 bytes (12 = 8 for double + 4 for Id). The minimum number of bits required to represent all entries can be calculated at the construction time and saved with the structure.  

The low level design should allow easy switching between representations. Some experimentation will be required to estimate memory/speed tradeoffs between different representations. 
 
**LonIndex**: a similar structure using longitude as the coordinate. 

The result of the search is converted into a **Location Filter**: a bitmap indexed by object Id with 1 denoting presence of object in the index.  

####LatLon search support
For simplicity, we will represent a circular area specified as ( **LatLon**, R ) with a square ( left = Lon-R/2, top = Lat+R/2, right = Lon+R/2, bottom = Lat-R/2 ). This expands the area by about 20% but allows to use the same fast location filtering as for bounding boxes. The potential extraneous hits will have distance to the center of the circle greater than R and can be filtered out at the scoring stage.   

####Full text search support
**TextIndex**: a prefix tree with all words extracted from all relevant properties of all objects in the dataset, after analysis and transformation. Payload is the set of tuples: object Id, property Id, position inside property (offset/length; for multi-value properties also index of the sub-property).
 
	Make a Location Filter
	Normalize search terms
	Objects found in the tree are immediately screened using the filter. The ones passing the filter are added to the result set  
	Repeat for all words, merging result sets (the same object can come in from multiple terms). 
	Traverse the result set to score and sort. Scoring is TBD. 

####Autocomplete support
Caching!! Hold on to the text+location of the previous call, results of search on full terms and position in prefix tree for the last word (complete or not). Keep results of the partial search separate from the full terms'. Destroy if the new text (sans last term) or location are different
 
	Do the search on full (all but last) terms. Save the result set.
	Search for the last term covers the entire subtree with the root corresponding to the term (as opposed to 
		full term where it is only the node matching the term). Save the root of the subtree.
	On the next call, if the last term is a continuation of the prior search's last term, refine search in 
		the tree starting from the saved root. 
	If the last term is new (and what was thought of as partial term last time is now full), use the save 
		subtree's root as the new full term, add to the memorized result set. Do a new partial search with the new last word.   

###Scoring
TBD

###Category search support
At this point, category search is not considered for implementation.

###Dataset
Format, construction, download, validation, opening
Data format has to be versioned. The dataset has to be validated at download time, or open time, or offline. 

##Analysis and transformation
Examine libpostal for its usefulness in (international) postal address normalization. Evaluate resource requirements and quality. 
Try to use the fact that normally the native language of the map is determined by the dataset's region.

For naive implementation, create a simplistic tokenizer akin to [https://www.elastic.co/guide/en/elasticsearch/reference/current/analysis-edgengram-tokenizer.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/analysis-edgengram-tokenizer.html) .

##Exception Handling wrapper
This C++ wrapper mirrors the original API in an exception-free manner. All methods will have specifier ```throw()```. 
In each method, the call to the corresponding original API in a try/catch clause. Use separate catch clauses for Pelopia-originated exceptions (if a part of the design). Use catch-all for all other exceptions. Convert exceptions into an enumeration of error codes. Return error code from each wrapper method. If the original method returns a value, make it an output parameter (the last).  

Save the text of the latest exception, provide a method to access it. Clear the saved exception text before any new call into the original API.

The library will not contain handlers for OS events. 

##Language bindings
The C+++ API can be used directly (see Exception handling below for a special case). For other languages, wrap the C++ library in an API usable from the target language.

###C wrapper
This wrapper consists of C global functions (prefixed with MzPel) directly derived from the original C++ APIl. The functions themselves can be implemented in C++ (make sure to specify "C" linkage). They will convert parameter and return values between C and corresponding C++ datatypes (e.g. char[]and string, void* and object). They will convert C++ exceptions into C return codes, perhaps by using the exception handling wrapper. 

###Android bindings 
Use JNI. Create an interface class, use javah to generate C header, implement header against the C++ API, possibly wrapped into the exception handling wrapper.

####Python bindings
Make a Python facade. Details TBD. 

####Node.js bindings
TBD

####Objective C / Swift bindings
TBD

##Logging
Support several levels of levels of logging (Debug, Info, Warning, Error). Provide a way to send log messages into a user-supplied logger. In the log messages, include time stamp, identify the source (class/method or function, filename, source line). Avoid multi-line and overly long messages.

##JSON library interface, JSON library
Use RapidJSON for JSON related needs.

##Examples

###C++

###Java

###Python

###Node.js

###Objective C

###Swift

##Testing
Develop test-first, unless there is a good reason not to,  

###Unit testing
Use Catch unit testing framework for writing C++ unit tests.
Find a test coverage tool. 

###Acceptance testing
For acceptance testing, capture some results from Pelias searches and, making sure the location criteria match, use them as expected results. Be prepared to update the expected results as Pelias evolves. Develop a simple acceptance test runner akin to Pelias's.

###Performance testing
Find a representative large dataset (NYC?) for performance measurement and experimentation.

###Integration testing
Create an integration test suite for every end-user API (C++, Android/Java, OSX/Swift/ObjC). The test have to be written against the API and exercise the subsystems end-to-end. Breadth of coverage is not as important.

##Build system
Investigate the C++ standard supported by all tool chains in use, document, set corresponding options across all compilers.

###MSVS
Use MS Visual Studio 2013.

###Linux (gcc/Makefiles)
Make sure any code developed for MS Visual Studio also works on Linux, by using continuous integration. Use g++ as the Linux compiler. 

###CMake
Investigate using CMake to generate MS Visual Studio project files and makefiles.

##Continuous integration
Find a CI tool that supports both MSVS and Linux makefiles. At a later stage, add CI for Android, iOS, OSX. 

##Documentation
Use doxygen for auto-generated documentation. Maintain README.md at the top level for (links to) everything else.

##Delivery
The deliverable is a set of header files and a static library.
 
Being the open source project, always consider that the deliverables can be built from sources. As an easier-to-use alternative, create .msi (.zip?) file for Windows, .rpm (tar?) for Linux, .jar for Android, ? for iOS/OSX.
Where required, the installation package has to include the C++ run time support library.


##Stories 

1. Search using one term text using BoundingBox, naive implementation
1. Search using one term text using LatLon, naive implementation
1. Search using multiple term text, naive implementation
1. Search using one incomplete term text, naive implementation 
1. Search using multiple complete term and one incomplete term text, naive implementation

1. Search using one term text using BoundingBox, optimized implementation
1. Search using one term text using LatLon, optimized implementation
1. Search using multiple term text, optimized implementation
1. Search using one incomplete term text, optimized implementation 
1. Search using multiple complete term and one incomplete term text, optimized implementation

1. Search using a dataset locally converted from Pelias-produced JSON  
1. Search using a dataset converted from dataset downloaded from a Pelopia-specific Mapzen endpoint
  
1. Search through a C API
1. Search through a Java API
1. Search through a Python API
1. Search through a Node.js API
1. Search through an Objective C API
1. Search through a Swift API

1. Installation and operation on Windows 
1. Installation and operation on Linux 
1. Installation and operation on Android 
1. Installation and operation on OSX 
1. Installation and operation on iOS 

1. Search using one category  
1. Search using multiple categories  

##Milestones

1. Unit testing framework is operational
1. Acceptance testing framework is operational
1. CI is operational, per platform
1. Windows API is feature complete (download from Mapzen, C++ API working)
1. Linux API is feature complete (download from Mapzen, C++ API working)
1. Android API is feature complete (download from Mapzen, Android API working)
1. OSX API is feature complete (download from Mapzen, C++ API working)
1. iOS API is feature complete (download from Mapzen, Objective C or Swift C API working)
2. Performance milestones met, per platform
 
