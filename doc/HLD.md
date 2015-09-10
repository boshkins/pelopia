#Pelopia, an offline geocoding library
#High Level Design

##Overview

This document describes high level design for Pelias, a native, offline geocoding solution over a relatively small set of data (e.g. a city). 
The design is based on requirements specified in [doc/requirements.md](https://github.com/boshkins/pelopia/blob/master/doc/requirements.md)

Read time containers are immutable, best represented as a buffer loaded into memory without any additional processing. Write side can analyze the structure and choose alternative representations or suggest more efficient lookup methods.

##Components

###API

All externally visible C++ declarations of the API will be contained in the namespace Mapzen :: Pelopia. Any non-C++ wrappers will use prefix MzPel to avoid naming conflicts, if namespaces are not available.

####Data:
**Data**: a sequence of serialized GeoJSON-derived objects. Properties are assigned Ids. **Data** can be kept on disk or in memory. The rest of the structures are in memory.

**Index**: an array of offsets into data, subscript is the object Id. Size of an Id value depends on the total size of the **Data**; 4 bytes should be enough but consider packing into less than 32 bits.   

####Bounding Box search support
**LatIndex**: an array of pairs "coordinate, object Id" sorted by coordinate (latitude). There may be multiple entries with matching coordinates.
 
The principal operation will be to extract the subset of points located between 2 coordinates [*low* .. *high*]:

	FindSegment ( CoordIndex, LowCoord, HighCoord ) : ( firstIdx, lastIdx )
		firstIdx = FindFirstGreaterThan ( CoordIndex, 0, size ( CoordIndex ), LowCoord )  
		lastIdx = FindLastLessThan ( CoordIndex, firstIdx, size ( CoordIndex ), HighCoord )  
		return ( firstIdx, lastIdx )

The coordinate will be initially represented as a double. The alternatives would be float and integer-based fixed point; it may make sense to decrease precision of the coordinate to save space in this structure. The full-precision coordinates extracted from the objects will then be used at the scoring stage to filter out false hits, if necessary. 

For additional memory savings at the cost of lookup speed, pairs "coordinate, object Id" may be packed into fewer than 12 bytes (12 = 8 for double + 4 for Id). The minimum number of bits required to represent all entries can be calculated at the construction time and saved with the structure.  

The low level design should allow easy switching between representations. Some experimentation will be required to estimate memory/speed tradeoffs between different representations. 
 
**LonIndex**: a similar structure using longitude as the coordinate. 

The result of the search is converted into a **Location Filter**: a bitmap indexed by object Id with 1 denoting presence of object in the index.  

####LatLon search support
For simplicity, we will represent a circular area specified as ( **LatLon**, R ) with a square ( left = Lon-R/2, top = Lat+R/2, right = Lon+R/2, bottom = Lat-R/2 ). This expands the area by about 20% but allows to use the same fast location filtering as for bounding boxes. The potential extraneous hits will have distance to the center of the circle greater than R and can be filtered out at the scoring stage.   

####Full text search support
**TextIndex**: a prefix tree with all words extracted from all relevant properties of all objects in the dataset, after analysis and transformation. Payload is the set of tuples: object Id, property Id, position inside property (what about multi-value properties?).
 
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

###Dataset
Format, construction, download, validation, opening
Data format has to be versioned. The dataset has to be validated at download time, or open time, or offline. 

###Analysis and transformation
Use libpostal as an (international) postal address normalization. Evaluate resource requirements and quality. 
Try to use the fact that normally the native language of the map is determined by the dataset's region.

###Language bindings
The C+++ API can be used directly (see Exception handling below for a special case). For other languages, wrap the C++ library in an API usable from the target language.

###Exception Handling wrapper
This C++ wrapper mirrors the original API in an exception-free manner. All methods will have specifier ```throw()```. 
In each method, the call to the corresponding original API in a try/catch clause. Use separate catch clauses for Pelopia-originated exceptions (if a part of the design). Use catch-all for all other exceptions. Convert exceptions into an enumeration of error codes. Return error code from each wrapper method. If the original method returns a value, make it an output parameter (the last).  

Save the text of the latest exception, provide a method to access it. Clear the saved exception text before any new call into the original API.

####C wrapper
This wrapper consists of C global functions (prefixed with MzPel) directly derived from the original C++ APIl. The functions themselves can be implemented in C++ (make sure to specify "C" linkage). They will convert parameter and return values between C and corresponding C++ datatypes (e.g. char[]and string, void* and object). They will convert C++ exceptions into C return codes, perhaps by using the exception handling wrapper. 

####Android bindings 
Use JNI. Create an interface class, use javah to generate C header, implement header against the C++ API, possibly wrapped into the exception handling wrapper.

####Python bindings
Make a Python facade. Details TBD. 

####Node.js bindings
TBD

####Objective C / Swift bindings
TBD

###Error handling
How to deal with OS signals? probably ignore.

###Logging
Support several levels of levels of logging (Debug, Info, Warning, Error). Provide a way to send log messages into a user-supplied logger. In the log messages, include time stamp, identify the source (class/method or function, filename, source line). Avoid multi-line and overly long messages.

###JSON library interface, JSON library
Use RapidJSON for JSON related needs.

##Testing
Develop test-first, unless there is a very good reason not to,  

###Unit testing
Use Catch unit testing framework for writing unit tests.
Find a test coverage tool. 

###Acceptance testing
For acceptance testing, capture some results from Pelias searches and, making sure the location criteria match, use them as expected. Be prepared to update the expected results as Pelias evolves. Develop a simple acceptance test runner akin to Pelias's.

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


##Stories and milestones




