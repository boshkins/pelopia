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

Each search operation will take place against a previously selected dataset. One of the ways to support that is to define a class representing datasets, and have the calling code create an instance of the class for each dataset in operation. The library must allow multiple datasets to be open at the same time. The design stage needs to address the tradeoff between speed of search and memory consumption which is likely to be required in the presence of multiple concurrently open datasets. 
    
The datasets will be downloaded via the API before they can be opened, using HTTP(S) and possibly converted into a local representation (GeoJSON or similar), as identified by the design.

For the sake of speed, the search is likely to be performed completely in-memory, which will result in a limitation on the maximum in-RAM size of the supported datasets. The limitation will be platform-specific, calculated and published for every target platform. 

The library must handle out-of-memory conditions gracefully in a variety of situations:

- when asked to download a dataset that exceeds the available storage (disk or other non-RAM memory) space
- when asked to download a dataset that would exceed the platform's in-RAM limit when opened
- when asked to open a dataset that would require more RAM than is available from OS at the moment
- when running out of RAM in the course of operation

##API

The operations identified in this section should be declared in a separate C++ namespace (pelopia); if they are implemented as methods on a class, so should be the class.

In the operations' descriptions below, the notation { X } is used to denote a possibly empty collection of objects of type X.  

The result sets (**Geocode**) returned by the search operations should be represented in conformance to [geocodejson-spec](https://github.com/geocoders/geocodejson-spec). In the context of a C++ API, this means a class providing read access to all properties prescribed by the specification, as well as a function/method to convert the object into a character string representing an equivalent GeocodeJSON object.

###Search ( string, categories, options ) : Geocode
####Options:
- LatLon, radius, or:
- Bounding box: bottom-left LatLon, top-right LatLon
- Size: max numbers of results to return
- Details: whether to search in all properties, or only in id/layer/text

Either string or categories must be present.
This operation can make the assumption that its text input is complete. 

###SearchCoarse ( string, options ) : Geocode
 Like Search above but will only search administrative regions (countries, states, counties, neighborhoods, etc.).

###Autocomplete ( string, options ) : Geocode

####Options
- LatLon
- Zoom level
- Size: max numbers of results to return
- Layers: dataset(s) to query
- Details: whether to search in all properties, or only in id/layer/text

Unlike Search, this operation will be called with incomplete text. It will also be called at a much higher rate than Search and must be sufficiently performant.  


###Reverse ( LatLon ) : Geocode
####Options
- Zoom level
- Layers: dataset(s) to query
- Details: whether to search in all properties, or only in id/layer/text

###Place ( { id } ) : Geocode


##Additional APIs

###Download ( bounding box, dataset_name )
Optionally compress ?

###Use ( { dataset_name } )
class constructor ?

###Configure ( { dataset_location } )


##Packaging and Installation

The library is an open source project. As such, it should be made available in source code form with corresponding project files to allow building it from source code on supported development systems. 
Additionally, the binary library should be packaged according to the specific platform's traditions (e.g. msi on Windows, rpm/deb on Linux, jar on Android). Debug builds of the library may be included.     

##Acceptance testing
Reuse the logic (and possibly data) of the stock acceptance tests for the online solution. Filter by coordinates?
Altenatively, use a 3d party acceptance testing framework

##External interfaces
###Software
May need 3d party libraries to deal with external representation of datasets. JSON is the likely first candidate. 
Here is a comparison of efficiency of some popular JSON libraries for C++: [https://blog.thousandeyes.com/efficiency-comparison-c-json-libraries/](https://blog.thousandeyes.com/efficiency-comparison-c-json-libraries/); more libraries are mentioned in the comments

###Communication
Datasets will be downloaded from the Internet via Download API from a number of supported endpoints, or supplied by the user in one of the supported data formats. 
HTTP(S) will be used to download datasets from a number of known endpoints. 
Consider downloading from a user-supplied URL, if the data format of the download is supported.

The list of supported endpoints should be easily configurable, maybe not by the user. 

The format of the download is likely to be (Geo)JSON, which may be converted into an internal format according to the needs of the library.

###Hardware
Target platforms are:
 
- Desktop: Windows, Linux; OSX (later). 
- Mobile: Android (ARM, Intel, MIPS); iOS (later).

TBD: Specify minimum supported version of corresponding operating systems.

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
Alternative JSON libraries: make sure any 3d party JSON libraries are wrapped in an interface, to facilitate switching between different libraries, be it for performance comparison or due to unavailability on a particular platform.

The same should apply to any other third party library in use. 
