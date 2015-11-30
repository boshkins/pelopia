#Pelopia, an offline geocoding library
#Requirements

##Overview

This project represents a native, offline geocoding solution over a relatively small set of data (e.g. a city).

The goal is to implement a set of features as close to the online solution, [Pelias](https://github.com/pelias/api), as practical.

The solution is to be cross-platform: Windows, Linux, Android; later on OS X, iOS.

The solution takes form of a C++ API, additionally wrapped for portability to other programming systems:

- Android (Java/JNI)
- Python
- Node.js
- iOS (Objective C, Swift) 

The library may be compiled to different target architectures (Intel/ARM/MIPS/...)

The library will provide two ways of lookup, by a text string (representing a complete or partial address, a name, or a category of locations) and by a combination of a latitude and a longitude (**LatLon**). 

Each search operation will take place against a previously selected dataset. One of the ways to support that is to define a class representing a dataset, and have the calling code create an instance of the class for each dataset in operation. 

At this point, the library is not required to support multiple datasets open at the same time.  
    
The datasets will be downloaded from a Mapzen-owned Web service before they can be opened, in a format identified in the design phase.

For the sake of speed, the search is likely to be performed completely in-memory, which will result in a limitation on the maximum in-RAM size of the supported datasets. The limitation will be platform-specific, calculated and published for every target platform. 

The library must handle out-of-memory conditions gracefully in a variety of situations:

- when asked to download a dataset that exceeds the available storage (disk or other non-RAM memory) space
- when asked to download a dataset that would exceed the platform's in-RAM limit when opened
- when asked to open a dataset that would require more RAM than is available from OS at the moment
- when running out of RAM in the course of operation

##Core API

The operations identified in this section should be declared in a separate C++ namespace (pelopia); if they are implemented as methods on a class, so should be the class.

In the operations' descriptions below, the notation ```{ X }``` is used to denote a possibly empty collection of objects of type **X**.  

The result sets (**Response**) returned by the search operations should be represented in conformance to [geocodejson-spec](https://github.com/geocoders/geocodejson-spec). In the context of a C++ API, this means a class providing read access to all properties prescribed by the specification, as well as a function/method to convert the object into a character string representing an equivalent GeocodeJSON object. Another possibility is to return object ID(s) only, with a separate function accepting an ID and returning a full-blown C++ object and/or a GeocodeJSON string representing it.

###Search ( *text*, *scope*, *format* ) : *Response*

Produces full text search results sorted by linguistic relevance to the given input string. Results should contain a quality value (e.g. [levenshtine distance](https://en.wikipedia.org/wiki/Levenshtein_distance) from the search string). 0 to 1.

####*text*:
A complete search string.  The string will be submitted to a 3d party address parser (current candidate: [https://github.com/openvenues/libpostal](https://github.com/openvenues/libpostal)). Based on the success rate of the address parser (to be established at the design stage), further attempts to analyze the text may be required. 
The parsed address string will be used to perform text search. Look at Pelias/elasticsearch/Lucene, build an in-memory specialization.   

####*scope*:
- **LatLon**, optional radius, or:
- Bounding box: bottom-left **LatLon**, top-right **LatLon**

####*format*:
- Size: max numbers of results to return. Optional; 10 if not specified. 

###Search ( *category*, *scope*, *format* ) : *Response*

Produces search results matching at least one of the specified categories. 

####*category*:
An array of string values such as POI types (restaurant, bank, education) and types of places (address, POI, city, country, state, etc).
See [category_map.js](https://github.com/pelias/openstreetmap/blob/master/config/category_map.js) for the current list of values. If the list changes frequently enough, build an auto-sync tool.

####*scope*, *format*:
Same as above.

**NOTE. At this point, category search is not considered for implementation.**

###Autocomplete ( *text*, *scope* ) : *Response*

Similar to Search on a text string. Unlike Search, this operation will be called with incomplete text. It will also be called at a much higher rate than Search and must be sufficiently performant.  

####*text*, *scope*.
Same as in Search. Format/size is fixed at 10.

###Reverse ( *LatLon*. *scope*, *format* ) : *Response*

####*scope*: 
- radius (specified in miles, kilometers, meters - all should be supported). Optional, unlimited (i.e. covering the entire dataset) by default

###Place ( { *ids* } ) : *{ Location }*

####ids: one or more ID
Converts specified IDs into corresponding C++ or stringified GeoJSON objects.
####Location: 
A C++ representation of GeoJSON object (possibly stringified) 

IDs are returned by other operations of the library (directly or as properties of C++ objects) and are opaque for the user. An ID uniquely identifies an object in its dataset.

##Additional APIs

The operations in this section do not have counterparts in Pelias but are needed here because of the "offline" nature of this project.
 
###Open ( { dataset_name } )

Open the specified dataset to use API operations on. This is likely to correspond to the class constructor.  

####*dataset_name*

An OS-specific path to the dataset file. 

##Packaging and Installation

The library is an open source project. As such, it should be made available in source code form with corresponding project files to allow building it from source code on supported development systems. 
Additionally, the binary library should be packaged according to the specific platform's traditions (e.g. msi on Windows, rpm/deb on Linux, jar on Android). Debug builds of the library may be included.     

##Acceptance testing
Reuse the logic (and possibly data) of the stock acceptance tests for the online solution. 
If feasible, use a 3d party acceptance testing framework

##External interfaces
###Software
May need 3d party libraries to deal with external representation of datasets. JSON is the likely first candidate. 
Here is a benchmark of a number of JSON libraries for C++: [https://github.com/miloyip/nativejson-benchmark](https://github.com/miloyip/nativejson-benchmark)

For address parsing, use [https://github.com/openvenues/libpostal](https://github.com/openvenues/libpostal)

###Communication
Datasets will be downloaded in advance from a Mapzen-maintained Web service. No network communication is expected from the library itself.  

###Hardware
Target platforms are:
 
- Desktop: Windows, Linux; OSX (later). 
- Mobile: Android (ARM, Intel, MIPS); iOS (later).

Specify minimum supported version of corresponding operating systems.

##Performance requirements
The Autocomplete operation, used to implement typeahead, is likely to be called on every keystroke and has to be fast enough to keep up with the speed of human typing, on average 200 characters per minute. 400 calls per minute is faster than most humans and can be considered a stretch target.

The speed of other calls is not as critical, but none should take more than 1-2 seconds. This can be problematic on less advanced platforms; there should be created a performance benchmarking test suite allowing a quick evaluation of library's performance on a given platform. 

##Error detection and recovery
The C++ library can throw exceptions internally and across its API. However, not all users welcome exceptions thrown by third party libraries, so there should be an optional wrapper that would convert exceptions raised by the library into a set of (numeric and/or textual) result codes.
Conversion of exception into result codes may be a necessary part of wrappers for different programming languages.

The library's code should create a log that can be used in performance and quality analysis, as well as troubleshooting. The log has to be created in a user-code specified location (with a reasonable default) and the level of detail of the log messages should be controllable by the user, with the a traditional set of levels (in the decreasing degree of detail: debug/info/warning/error/off). Some of the logging levels may not be available in non-debug builds. Care should be taken to ensure that logging logic has minimal effect on performance when logging is not requested. 

##Privacy
The library should not store user queries in any long-term memory past execution of the query. One exception is the execution log, when a sufficiently deep level of detail (debug or info) is requested by the user. The packaging has to include a Privacy policy statement mentioning the relevant details.

##Machine memory and free disk space
The minimum requirements will be determined as part of the project, possibly on a platform-by-platform basis. The maximum supported dataset size, as mentioned above, will also have to be estimated and published.

##Maintainability
The design needs to identify address areas that are expected to be changed more frequently than others. For such areas, a clean decoupling between components is required. 

###Changes in functionality
This library should mirror the evolution of the Pelias project. 
TBD: identify areas of Pelias most likely to undergo changes. 

###Changes in operating environment
The core library has to be portable and platform-agnostic. Use only standard features of C++ (determine which standard to adhere to, be it  [C++98, C++03, etc](https://en.wikipedia.org/wiki/C%2B%2B) ). If any OS-specific code has to be written, hide it behind an abstract interface and keep separate from platform-independent modules. 
 
###Changes in interfaces
3d party libraries: make sure any external libraries are wrapped in an interface, to facilitate switching between different libraries, be it for performance comparison or due to unavailability on a particular platform.

