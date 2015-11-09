#Pelopia, an offline geocoding library
#High Level Design

##Overview

This document describes high level design for Pelopia, a native, offline geocoding solution over a relatively small set of data (e.g. a city). 
The design is based on requirements specified in [doc/requirements.md](https://github.com/boshkins/pelopia/blob/master/doc/requirements.md)

The structure of the document is as follows:

- A list of API operations and supporting datatypes, in a notation close to C++
- Interfaces supporting API operations, with outlines of their naive (quick-and-dirty) implementations, as well as more size and/or speed efficient approaches.
- Pseudocode for the implementation of the API operations using the interfaces described earlier
- Examples of client code using the API
- Notes of testing, integration, multi-platform aspects etc
- A set of stories to act as the initial backlog for the project
- A list of implementation milestones  

##References
1. C.Gormley, Z.Tong. Elasticsearch: The Definitive Guide. [https://www.elastic.co/guide/en/elasticsearch/guide/master/index.html](https://www.elastic.co/guide/en/elasticsearch/guide/master/index.html)
2. Filip Janiszewski. A Lightweight Logger for C++ [http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505](http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505)

##API operations

All externally visible C++ declarations of the API will be contained in the namespace Mapzen :: Pelopia. Any non-C++ wrappers will use prefix MzPel to avoid naming conflicts, if namespaces are not available.

### Supporting data types

	typede uint32_t ResultCount;

	typedef struct { 
		ResultCount maxResults; 
	} Format;
	const ResultCount AllResults = 0;
	const ResultCount DefaultResults = 10;

	typedef double Coordinate; // degrees; underlying datatype is likely to change

	struct LatLon {
		Coordinate lat;	// range -180 .. 180 
		Coordinate lon; // range -180 .. 360; the extended range is used to handle discontinuity
	};					//  at the 180 meridian
 
	class BoundingBox {
	public:
		// all constructors will recalculate right longitude to above 180 degrees if the 180 
		//  meridian lies within the box
		BoundingBox ( LatLon p_topLeft, LatLon p_bottomRight );
		BoundingBox ( Coordinate latLeft, 
					  Coordinate latRight, 
					  Coordinate lonBottom, 
					  Coordinate lonTop );
	
		const Coordinate Left() const;
		const Coordinate Right() const;
		const Coordinate Top() const;
		const Coordinate Bottom() const;

		const LatLon& TopLeft() const;
		const LatLon& BottomRight() const;

	private:
		LatLon topLeft;
		LatLon bottomRight;
	};
 
	class Distance 
	{
	public:
		typedef enum { Miles, Kilometers } Units;

		Distance ( Units, double );

		double GetMiles () const;
		double GetKiloMetres () const;
	};

	typedef string Category;

	typedef uint64_t Id;
	const Id InvalidId = 0;

	// Match score, in the range 0 .. 1
	typedef double MatchQuality;

	class Response
	{	// a collection of results, sorted by descending quality 
	public:
		// the number of entries ion the response
		usigned int Count() const;

		// access location Id and its score by its position in the collection, 
		//		0 <= index < Count
		// returns false if index is outside of [ 0, Count ) 
		bool Get ( unsigned int index, Id & id, MatchQuality & score ) const;

		// access location Id by position in the collection, 0 <= index < Count
		// returns InvalidId if index is outside of [ 0, Count )
		Id Get ( unsigned int index ) const;

		// access the best-scoring completion for the corresponding entry's text, 
		// if requested in the original query (i.e. Dataset :: Autocomplete called)
		// returns empty string if index is outside of [ 0, Count )
		string Autocomplete ( unsigned int index ) const; 
	}

### API methods

	class Dataset {

	public:
		Dataset ( const char* filename ); 
 
		Response Search ( const char* text, 
						  const LatLon& scope,
						  const Distance& radius = Distance ( Miles, 0 ), 
						  Format format = { DefaultResults } );  

		Response Search ( const char* text, 
						  const BoundingBox& scope, 
						  Format format = { DefaultResults } );  
	
		Response Autocomplete (const char* text, const LatLon& scope );  
	
		Response Autocomplete ( const char* text, const BoundingBox& scope );  
	
		Response Reverse ( const LatLon& point, 
						   const Distance& radius, 
						   Format format = { AllResults } );

		GeoJSON::Feature Place ( Id ) const; 
	};

### GeoJSON objects

	namespace GeoJSON {
		// GeoJSON declarations, based on 
		// http://geojson.org/geojson-spec.html](http://geojson.org/geojson-spec.html

		class Geometry
		{	// for now, only Point in Geographic Coordinate Reference System. 
			// In the future, expand to polygons (open or closed)  
		public:
			Coordinate Latitude () const;
			Coordinate Longitude () const;
		}

		class Feature
		{	// This is a GeoJSON Feature object corresponding to a single Location 
		public:
			typedef enum 
			{
				Property_Layer,
				Property_Name,
				Property_Text,
				Property_Address,
				// ... etc - keep synchronized with the access methods below
			} PropertyId;

		public:

			const GeometryObject* Geometry() const;

			// Feature access, based on 
			// https://github.com/pelias/schema/blob/master/mappings/document.js

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

			// TBD: represent Shape

			const char* GetProperty ( PropertyId ) const;

			unsigned int CategoryCount () const;
			const char* Category ( unsigned int index ) const; 
			// consider adding CategoryId to work with enum-like categories

			string Stringify () const;	
		};

	}


##Supporting interfaces and algorithms

It is important to note that read time containers are (at least conceptually) immutable, so they are best represented as a buffer loaded into memory without any additional processing. 
Write side can analyze the structure and choose alternative representations or suggest more efficient lookup methods.

The usefulness of naive implementations is in their simplicity, allowing for a rapid development of a functionally complete system. The quickly-built system then can be used to explore functionality issues, as well as investigated for performance bottlenecks. The feedback from performance study will drive replacement of naive designs with more performant ones. Care must be taken to insulate the rest of the systems from the details of naive/efficient designs, which is achieved through the use of interfaces.

Since the implementation language is C++, interfaces are modeled by abstract classes. Alternatively, use Pimpl idiom to hide implementation details.

###Data:
**Data**: a sequence of GeoJSON-derived objects. 
A naive implementation would use an STL vector of in-memory GeoJSON::Location objects.

	class Data 
	{
	public:
		virtual const GeoJSON::Location& Get ( Id ) const = 0;
		virtual size_t Count () const = 0;
	};

A more memory-efficient representation would store serialized objects in a byte buffer (in memory or on disk) and involve an index:

**Index**: an array of offsets into data buffer; subscript is the object Id. Size of an Id value depends on the total size of the **Data**; 4 bytes should be enough but consider packing into less than 32 bits.

	class Index 
	{
	public:
		typedef size_t Offset;

	public:
		virtual Offset Get ( Id ) const = 0;
	};   

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

To support search filtered by a BoundingBox, we need an ability to filter locations. Two different cases involve locations specified by a single point (a LatLon value) and by a geoshape ( an open or closed polygon ).

**PointLatIndex**: an array of pairs "coordinate, object Id" sorted by coordinate (latitude). This index will only contain objects with location specified as a point. There may be multiple entries with matching coordinates.
 
The principal operation will be to extract the subset of points located between 2 coordinates [*low* .. *high*]:

	FindSegment ( CoordIndex, LowCoord, HighCoord ) : ( firstIdx, lastIdx )
		firstIdx = FindFirstGreaterThanOrEqual ( CoordIndex, 0, size ( CoordIndex ), LowCoord )  
		lastIdx = FindLastLessThanOrEqual ( CoordIndex, 
											firstIdx, 
											size ( CoordIndex ), 
											HighCoord )  
		return ( firstIdx, lastIdx )

The coordinate will be initially represented as a double. The alternatives would be float and integer-based fixed point; it may make sense to decrease precision of the coordinate to save space in this structure. The full-precision coordinates extracted from the objects will then be used at the scoring stage to filter out false hits, if necessary. 

For additional memory savings at the cost of lookup speed, pairs "coordinate, object Id" may be packed into fewer than 12 bytes (12 = 8 for double + 4 for Id). The minimum number of bits required to represent all entries can be calculated at the construction time and saved with the structure.  

The low level design should allow easy switching between representations. Some experimentation will be required to estimate memory/speed tradeoffs between different representations. 
 
**PointLonIndex**: a similar structure using longitude as the coordinate. 

####LatLon search support
For simplicity, we will represent a circular area specified as ( **LatLon**, R ) with a square ( left = Lon-R/2, top = Lat+R/2, right = Lon+R/2, bottom = Lat-R/2 ). This expands the area by about 20% but allows to use the same fast location filtering as for bounding boxes. The potential extraneous hits will have distance to the center of the circle greater than R and can be filtered out later, possibly as late as the scoring stage.

Note that LatLon search with the default radius covers the entire dataset. Filtering in this case can be bypassed, or the corresponding filter can be represented with a bitmap with all bits set to 1.   

####Locations with point geometry
Initially, all locations in the dataset will be represented as a single point ( centroid for lines and polygons, see [https://en.wikipedia.org/wiki/Centroid](https://en.wikipedia.org/wiki/Centroid ). 

The result of the search is converted into a **Location Filter**: a bitmap indexed by object Id with 1 denoting presence of object in the index.

	class LocationFilter //naive: : public vector < bool > 
	{
	public:
		LocationFilter ( size_t count ); // reserve and 0-initialize
		LocationFilter& and ( const LocationFilter& ); 
	};  


Building an index for a given bounding box involves calculating an intersection between two sets of locations, corresponding to lookups of **PointLatIndex** and **PointLonIndex**:

	MakeFilter ( dataset, bbox ) : LocationFilter

		( first, last ) = PointLatIndex . FindSegment ( bbox.bottom, bbox.top )
		LocationFilter lat ( TotalObjects );
		for each i in (first, last)
			lat [ i ] = true;

		LocationFilter lon ( TotalObjects );
		( first, last ) = PointLonIndex . FindSegment ( bbox.left, bbox.right )
		for each i in (first, last)
			lon [ i ] = true;

		// combine lat and lon filters
		return lat . and ( lon )

####Locations with shaped geometry
For location with shaped geometry, we will represent the object's location with a bounding box fully covering the shape (left = minimum longitude of the shape, top = maximum latitude, right = maximum longitude, bottom = minimum latitude). For such locations, the filtering function will be more complex and **TBD**.


####Geo distance calculations

The distance between two points will be calculated assuming that the world is flat, which should be sufficient for relatively small areas. As an option, we can consider implementing more computationally expensive but more accurate Haversine formula [https://en.wikipedia.org/wiki/Haversine_formula](https://en.wikipedia.org/wiki/Haversine_formula). For now, the distance is calculated using the Pythagorean theorem:

	Distance ( LatLon ( lat1, lon1 ), LatLon ( lat2, lon2 ) ) = 
											sqrt ( ( lat2 - lat1 ) ** 2 + ( lon2 - lon1 ) ** 2 )  

####Full text search support
**TextIndex**: a prefix tree with all words extracted from all relevant properties of all objects in the dataset, after analysis and transformation. Payload is a set of tuples: object Id, property Id, position inside property (offset/length; for multi-value properties also index of the sub-property).
 
	Make a Location Filter
	Normalize query: remove stop words, break into terms (or use an address normalization 
		library)
	for each term in the query
		locate the tree node corresponding to the term
		for each object attached to the node
			Objects found in the tree are immediately screened using the filter. 
		The ones passing the filter are added to the result set  
	Repeat for all words, merging result sets (the same object can come in from multiple terms; 
		same term can be used more than once in the same object). 
	For each location in the result set, calculate its score. 
	Sort the result set by decreasing score.

The interfaces:

	class TextIndex 
	{
	public:
		class TermUse   
		{
		public:
			virtual Id ObjectId () const = 0;
			virtual GeoJSON :: PropertyId PropertyId () const = 0;
			virtual void Position ( size_t& index, size_t& offset, size_t& length ) const = 0; 
		}

		class Payload // naive: vector < TermUse >
		{	// the collection is sorted by increasing object Id, then by increasing index, then by increasing position  
			virtual size_t Count () const = 0;
			virtual const TermUse* GetTerm ( size_t index ) const = 0;
		}

		class Node
		{
		public:
			virtual const Payload& GetPayload () const = 0;

			virtual size_t ChildrenCount () const = 0;
			virtual const Node* GetChild ( size_t index ) const = 0;
		};

	public:
		virtual const Node& Locate ( const string& prefix ) const = 0;
	}

####Autocomplete support
Caching!! Hold on to the text+location of the previous call, results of search on full terms and position in prefix tree for the last word (complete or not). Keep results of the partial search separate from the full terms'. Destroy if the new text (sans last term) or location are different
 
	Do the search on full (all but last) terms. Save the result set, with scores.

	Search for the last term covers the entire subtree with the root corresponding to 
		the term (as opposed to full term where it is only the node matching the term). 
		Save the root of the subtree.

	On the next call, if the last term is a continuation of the prior search's last term, 
		refine search in the tree starting from the saved root. 
	If the last term is new (and what was thought of as partial term last time is now full), 
		use the save subtree's root as the new full term, add to the memorized result set. 
		Do a new partial search with the new last word.   

	**................................... incomplete**

###Scoring

The scoring approach combines Term Frequency / Inverse document frequency with geo scoring, both similar to the approach utilized by elasticsearch / Lucene. 

There should be a mechanism for overriding the library's internal scoring function. The user-supplied function should have access to all the inputs that the internal one does (i.e values used in the formulas below).

To limit the amount of calculations, scoring is done after applying location filtering.

####Full text search

For each term in a tokenized full text query, calculate its WEIGHT:

Term Frequency:
 
	TF = sqrt ( # of appearances of the term in the field )
 
Inverse document frequency: the more often the term appears in the dataset, the less relevant it is. This value can be pre-calculated for every term in the dataset, at the cost of increased memory footprint of the dataset. 

	IDF = 1 + log ( totalFields / ( 1 + # of fields containing the term ) )   

NOTE. Currently, by "field" in this section we understand "address". However, that may include other GeoJSIN fields, depending on the answer to the
# **Big question: which properties are we searching on?**  #


Field-length norm: the more words in the field, the lower relevance of words in it:

	FIELD_NORM = 1 / sqrt ( # of terms in the field )

The weight of the term in a query:

	WEIGHT = TF * IDF * FIELD_NORM


For each field in the candidate set created by filtering, calculate its SCORE in relation to the current query:

Query Normalization factor:

	QUERY_NORM = 1 / sqrt ( sum ( IDF of each term in the query ) )

Query coordination: rewards fields that contain a higher percentage of query terms.

	COORD = # of matching terms in the field / total terms in the query

Scoring function, similar to Lucene's Practical Scoring Function:

	SCORE = QUERY_NORM * COORD * sum ( WEIGHT,  for each term in query ) 

In addition, boost score of fields where the order of found terms matches the order of terms in the query. Details **TBD**

###Reverse search support
**TBD**

####Geo scoring
The purpose of the geo-scoring stage is twofold. 

First, the coordinates of retrieved locations are used to filter out false hits appearing due to representation of circles and polygons with an enclosing bounding box, both on the (circular) search region's and on the (shaped) location's side. 

Second, geographical distance between the location and the center of the search region is used as a tie breaker for elements of the result set with matching scores. For a shaped location (when supported), it can be the distance between the center of the search region and the location's centroid. Alternatively, and more expensively calculation-wise, it can be the distance between the center of the search region and the location's nearest point to it.

####Scoring API

	class Scorer
	{
	public:
		virtual MatchQuality Score ( Dataset &, const vector < string > & query, const LatLon & center ) = 0;
	} 

###Dataset
Originally, dataset will be represented as a plain JSON file, which will be parsed and converted into library's internal structures at the Dataset object initialization time. For large datasets that is likely to become prohibitively slow, at which point we will have to introduce an intermediate representation (**IR**) used to store dataset in a more ready-to-consume (and external memory-efficient) format than JSON.

Conversion of JSON datasets into their **IR**s will ideally happen on a (MapZen) server, so that the dataset arrives to the user's device ready for use without further transformation. Alternatively, transformation can take place on the user's device post-download, via a utility program. 

To account for these alternatives, the transformation code has to be decoupled from the search portion of the library, so that it can packaged for autonomous use on server or post-download.

Note that address normalization is a part of converting dataset from MapZen-created GeoJSON into Pelopia's internal structures. 
 
To reduce the footprint on the user's device and, in the case of server-side transformation, the size of transmission, the **IR** should involve compression.

The library should provide some degree of validation of the dataset it is offered.

**IR** has to be versioned. Backward compatibility with older versions of the **IR** should be preserved.

##Analysis and transformation
Examine libpostal for its usefulness in (international) postal address normalization. Evaluate resource requirements and quality. 
Try to use the fact that normally the native language of the map is determined by the dataset's region.

For naive implementation, create a simplistic tokenizer akin to [https://www.elastic.co/guide/en/elasticsearch/reference/current/analysis-edgengram-tokenizer.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/analysis-edgengram-tokenizer.html) .

The same address normalization logic has to be applied to addresses as part of the dataset preparation process and to queries before search.

##Exception Handling wrapper
This C++ wrapper mirrors the original API in an exception-free manner. All methods will have specifier ```throw()```. 
Each method will consist of a call to the corresponding method from the original API in a try/catch clause. Use separate catch clauses for Pelopia-originated exceptions (if a part of the design). Use catch-all for all other exceptions. Convert exceptions into an enumeration of error codes. Return error code from each wrapper method. If the original method returns a value, make it an output parameter (the last).  

Save the text of the latest exception, provide a method to access it. Clear the saved exception text before any new call into the original API.

The library will not contain handlers for OS events. 

##Language bindings
The C+++ API can be used directly (see Exception handling below for a special case). For other languages, wrap the C++ library in an API usable from the target language.

###C wrapper
This wrapper consists of C global functions (prefixed with MzPel) directly derived from the original C++ APIl. The functions themselves can be implemented in C++ (make sure to specify "C" linkage). They will convert parameter and return values between C and corresponding C++ datatypes (e.g. char[]and string, void* and object). They will convert C++ exceptions into C return codes, perhaps by using the exception handling wrapper. 

###Android bindings 
Use JNI. Create an interface class equivalent to the C++ API, use javah to generate a C header, implement the functions declared in the header against the C++ API (possibly wrapped into the exception handling wrapper). 
Details **TBD**. 

####Python bindings
Make a Python facade. Details **TBD**. 

####Node.js bindings
**TBD**

####Objective C / Swift bindings
**TBD**

##Logging
Support several levels of levels of logging (Debug, Info, Warning, Error). Provide a way to send log messages into a user-supplied logger. In the log messages, include time stamp, identify the source (class/method or function, filename, source line). Avoid multi-line and overly long messages.

Follow the design described [here](http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505)

##JSON library interface, JSON library
Use RapidJSON for JSON related needs.

##Multithreading
On platforms with multiple CPU cores, multithreading can be a valuable performance boosting tool. In the design presented here, multiple threads can be beneficially used in two tasks: filtering and scoring. The general approach would be creating a pool of worker threads consuming location Ids off a common queue and saving results (ID + boolean for filtering, Id + double for scoring) in an output structure. Access to both input and output structures has to be thread safe. Use STL mutex-based locking for access synchronization.  

CPU cores are a valuable resource and the use of multithreading inside a third-party library may not be welcomed by the primary application.
Expose the mutlithreading switch, as well as the number of worker threads as a configuration parameter. For the default value of the number of threads,  use the number of CPU cores [reported by system](http://www.cprogramming.com/snippets/source-code/find-the-number-of-cpu-cores-for-windows-mac-or-linux), minus 2. If there are 2 cores or fewer, turn multithreading off.

##Directory structure

The contents of directory **pelopia/**:

	build/
		make/ 		- makefiles for gmake
		MSVS2013/ 	- solution/project files for MS Visual Studio 2013
		... 		- other build systems (Android, Cmake, XCode etc)
	doc/			- documentation
		html/		- doxygen-generated HTML documentation
		*.md		- other documents, in MarkDown
	examples/
		C++/		- C++ examples
		C/			- C examples
		...			- other languages
	inc/
		pelopia/	- pelopia headers
		*.h 		- 3d party headers, as needed
	src/
		test/		- unit tests
		*.cpp 		- library code
	acceptance/		- acceptance tests

##Examples

###C++

Using Search:

	#include <iostream>
	
	#include <pelopia/Dataset.h>
	#include <pelopia/GeoJSON.h>
	
	using namespace Mapzen::Pelopia;
	using namespace std;
	
	void main()
	{
		Dataset ds("NYC.pelopia");
	
		Response resp = ds.Search("Mapzen", LatLon(40.7507545, -73.9939152));
	
		for (unsigned int i = 0; i < resp.Count(); ++i)
		{
			Id id;
			MatchQuality score;
			resp.Get(i, id, score);
			GeoJSON::Feature place = ds.Place(id);
			cout << (i + 1) << " ( " << score << " ) "
				<< ": " << place.Name() << ", " << place.Address()
				<< endl;
		}
	}


###Java
**TBD**

###Python
**TBD**

###Node.js
**TBD**

###Objective C
**TBD**

###Swift
**TBD**

##Testing
Develop test-first, unless there is a good reason not to.   

###Unit testing
Use Catch unit testing framework for writing C++ unit tests.
Find a test coverage tool. Start [here](http://open-tube.com/10-code-coverage-tools-c-c/) or [here](https://www.quora.com/What-are-some-code-coverage-tools-for-C++)

###Acceptance testing
For acceptance testing, capture some results from Pelias searches and, making sure the location criteria match, use them as expected results. Be prepared to update the expected results as Pelias evolves. Develop a simple acceptance test runner akin to Pelias's.

###Performance testing
Find a representative large dataset (NYC?) for performance measurement and experimentation. Identify an execution profiling tool for every platform.  Make sure to measure speed on fully optimized release builds.   

###Integration testing
Create an integration test suite for every end-user API (C++, Android/Java, OSX/Swift/ObjC). The test have to be written against the API and exercise the subsystems end-to-end. Breadth of coverage is not as important. For a testing framework, use Catch for C/C++, JUnit for Java, XCTest for Objective C and Swift. 

##Build system
Investigate the C++ standard supported by all tool chains in use, document, set corresponding options across all compilers.

###MSVS
Use MS Visual Studio 2013.

###Linux (gcc/Makefiles)
Make sure any code developed for MS Visual Studio also works on Linux, by using continuous integration. Use g++ as the Linux compiler. Identify the minimum supported version of gcc.

###CMake
Investigate using CMake to generate MS Visual Studio project files and makefiles.

##Continuous integration
Ideally, use one CI tool that supports MSVS projects (Windows) and gmake makefiles (Linux/OSX), as well as Python and node.js. At a later stage, add CI for Android, iOS.  

##Documentation
Use doxygen for auto-generated documentation on the declarations included in the API. Maintain README.md at the top level for (links to) everything else.

##Delivery
The deliverable is a set of header files, a static library and documentation.
 
Being the open source project, always consider that the deliverables can be built from sources. As an easier-to-use alternative, create .msi (.zip?) file for Windows, .rpm (tar?) for Linux, .jar for Android, ? for iOS/OSX.
Where required, the installation package has to include the C++ run time support library.

Contents of a Pelopia installation directory:

	pelopia/
		inc/		- header files
		lib/		- binary libraries
		doc/		- HTML documentation
		examples/
			C++/
			C/
			...		- other languages, as needed

An alternative installation structure, for Unix-like systems with root access, would be

	/usr/local/
		include/pelopia/		- headers
		lib/pelopia/			- libraries
		share/doc/pelopia/		- HTML documentation
					/examples	- examples

##Stories 

1. Logging to a file
1. Logging to a client-supplied stream
 
1. Use a dataset from Mapzen-produced JSON
  
1. Search using one term text using BoundingBox, naive implementation
1. Search using one term text using LatLon, naive implementation
1. Search using multiple term text, naive implementation
1. Search using one incomplete term text, naive implementation 
1. Search using multiple complete term and one incomplete term text, naive implementation
2. Autocomplete, naive
2. Reverse search, naive

1. Use a dataset locally converted from Mapzen-produced JSON
2.   
1. Search using one term text using BoundingBox, optimized implementation
1. Search using one term text using LatLon, optimized implementation
1. Search using multiple term text, optimized implementation
1. Search using one incomplete term text, optimized implementation 
1. Search using multiple complete term and one incomplete term text, optimized implementation
2. Autocomplete, optimized
2. Reverse search, optimized

1. Use a dataset downloaded from a Pelopia-specific Mapzen endpoint in Pelopia format

1. Exception handling wrapper
 
1. Search through a C API
1. Search through a Java API
1. Search through a Python API
1. Search through a Node.js API
1. Search through an Objective C API
1. Search through a Swift API

1. Download/build from sources in a clean environment on Windows 
1. Download/build from sources in a clean environment on Linux 
1. Download/build from sources in a clean environment on Android 
1. Download/build from sources in a clean environment on OSX 
1. Download/build from sources in a clean environment on iOS 

1. Installation package for Windows 
1. Installation package for Linux 
1. Installation package for Linux, root mode 
1. Installation package for Android 
1. Installation package for OSX 
1. Installation package for iOS 

##Milestones

2. Search with LatLon works
2. Search with BoundingBox works
3. Autocomplete works
3. Reverse search works

1. API is feature complete (JSON downloaded from Mapzen, C++ API working), per platform

2. API works using datasets in optimized format prepared locally, all platforms
2. API works using datasets in optimized format prepared on server, all platforms

3. C API
3. Java API 
3. Python API 
3. Node.js API 
3. Swift API 
3. Objective C API 
  
1. Performance milestones met, per platform

##Non-story tasks
1. Set up unit testing framework
1. Set up acceptance testing framework
1. Set up CI, per platform/language
2. Set up Doxygen

