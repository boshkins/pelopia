#Pelopia, an offline geocoding library
#High Level Design

##Overview

This document describes high level design for Pelias, a native, offline geocoding solution over a relatively small set of data (e.g. a city). 
The design is based on requirements specified in [doc/requirements.md](https://github.com/boshkins/pelopia/blob/master/doc/requirements.md)

Read time containers are immutable! Write side can analyze the structure and choose alternative representations or suggest more efficient lookup methods.

##Components

###API

All externally visible C++ declarations of the API will be contained in the namespace Mapzen :: Pelopia. Any non-C++ wrappers will use prefix MzPel to avoid naming conflicts. 

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
		subtree's root as the new ful term, add to the memorized result set. Do a new partial search with the new last word.   

###Dataset
Format, construction, download, validation, opening
Data format has to be versioned. The dataset has to be validated at download time, or open time, or offline. 

###Analysis and transformation	

###Language bindings

####C wrapper

####Android bindings 

####Python bindings

####Node.js bindings

####Objective C / Swift bindings

###Exception Handling wrapper

###Error handling

###Logging

###JSON library interface, JSON library

##Testing

###Unit testing

###Acceptance testing

###Performance testing
Find a representative large dataset (NYC?) for performance measurement and experimentation.

###Integration testing

##Build system
###MSVS
###Linux (gcc/Makefiles)
###Use CMake?

##Continuous integration

##Documentation

##Delivery

##Stories and milestones




