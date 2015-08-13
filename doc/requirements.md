#Pelopia, an offline geocoding library. 
#Requirements.

##Overview

This project represents a native (offline) geocoding solution over a relatively small set of data (e.g. a city).

Has to be cross-platform: Windows (Linux? OS X?), iOS, Android.
(Almost) parity of features with the online solution.

A C++ API, additionally wrapped in C for portability to iOS (Objective C, Swift) or Android (Java/JNI, possibly targeted to ARM/Intel/MIPS)

Two different data structures to support lookup by string and by lat/lon. The structures are local, probably in-memory.

The data will be downloaded in advance (or as a separate API operation), using (Geo)JSON as the local representation.

##API
###Search ( string, options ) : { string }
####Options:
- Lat, lon
- Zoom level
- Size: max numbers of results to return
- Layers: dataset(s) to query
- Bounding box: bottom-left lat/lon, top-right  lat/lon
- Details: whether to search in all properties, or only in id/layer/text

###SearchCoarse ( string, options ) : { string }
 Like Search above but will only search administrative regions (countries, states, counties, neighborhoods, etc.).

###Suggest ( string, options ) : { string }
####Options
- Lat, lon
- Zoom level
- Size: max numbers of results to return
- Layers: dataset(s) to query
- Details: whether to search in all properties, or only in id/layer/text

###SuggestCoarse ( string, options ) : { string }
Like Suggest above but will only search administrative regions

###SuggestNearby ( string, options ) : { string }
Like Suggest above but works as autocomplete for only the places located near a latitude/longitude; this endpoint is the same as /suggest but the results are all from within 50 kilometers of the specified point. Unlike suggest, suggestNearby does not mix results from different precision levels (500km, 1000km etc from lat/lon).

###Reverse ( lat/lon ) : { string } – or a single string ?
####Options
- Zoom level
- Layers: dataset(s) to query
- Details: whether to search in all properties, or only in id/layer/text

###Doc ( { id } ) : { string } – or structures? Json values?

##Additional APIs
###Download ( bounding box, dataset_name )
Optionally compress ?

###Use ( { dataset_name } )

###Configure ( { dataset_location } )

##Acceptance testing
Reuse the logic (and possibly data) of the stock acceptance tests for the online solution. Filter by coordinates?
Altenatively, use a 3d party acceptance testing framework

##External interfaces
###Software
JSON for C++ https://blog.thousandeyes.com/efficiency-comparison-c-json-libraries/ - benchmarking of some popular libraries; more libraries are mentioned in the comments
Extend for GeoJSON ?

###Communication
HTTP(s) used to download datasets
###Hardware
Identify target platforms. Desktop: Windows/Linux/OS X. Mobile: Android (ARM, Intel, MIPS), iOS. 
Specify minimum supported version of corresponding operating systems.

##User tasks
###Input
TBD
###Output
TBD

##Performance requirements
###Expected response times
TBD
###Processing time
TBD
###Data transfer rate
TBD
###System throughput

##Reliability
###Error detection and recovery
TBD

##Security
TBD

##Minimum machine memory and free disk space
TBD

##Maintainability
###Changes in functionality
TBD
###Changes in operating environment
TBD
###Changes in interfaces
Alternative JSON libraries

