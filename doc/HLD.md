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

```c++
typede uint32_t ResultCount;

typedef struct {
	ResultCount maxResults;
} Format;
const ResultCount AllResults = 0;
const ResultCount DefaultResults = 10;

typedef double Coordinate; // degrees; underlying datatype is likely to change

// class LatLon:
// Latitude in the range of -90 .. 90
// Longitude in the range -180 .. 360; the extended range may be used to handle discontinuity
//                                      at the 180 meridian
// the constructor and setters will throw if latitude / longitude is outside of its range
class LatLon 
{
public:
	LatLon ( Coordinate p_lat, Coordinate p_lon ) throw ( std :: logic_error ); 

	Coordinate Latitude  () const;
	Coordinate Longitude () const;
    
	void SetLatitude  ( Coordinate )  throw ( std :: logic_error );
	void SetLongitude ( Coordinate )  throw ( std :: logic_error );
}
            
class BoundingBox {
    // All 4-Coordinate calls follow the order
    // Top - Left - Bottom - Right
    //
    // All 2-LatLon calls follow the order
    // TopLeft - BottomRight

	// all constructors will recalculate longitudes to 0 .. 540 if the 180 
	//  meridian lies within the box
	BoundingBox (); // all 0s
	BoundingBox ( LatLon p_topLeft, LatLon p_bottomRight )  throw ( std :: logic_error );
	BoundingBox (
		Coordinate latTop, 
        Coordinate lonLeft,
		Coordinate latBottom,
		Coordinate lonRight )  throw ( std :: logic_error );

	Coordinate Top() const      { return m_topLeft . Latitude (); }
	Coordinate Left() const     { return m_topLeft . Longitude (); }
	Coordinate Bottom() const   { return m_bottomRight . Latitude (); }
	Coordinate Right() const    { return m_bottomRight . Longitude (); }

	const LatLon& TopLeft() const { return m_topLeft; }
	const LatLon& BottomRight() const { return m_bottomRight; }
    
	// the setters take care of translation of longitude around the 180 meridian, if necessary
	void SetCoordinates ( const LatLon& p_topLeft,  const LatLon& p_bottomRight )  
	   throw ( std :: logic_error );
	void SetCoordinates ( 
		Coordinate latTop, 
		Coordinate lonLeft,
		Coordinate latBottom,
		Coordinate lonRight )  throw ( std :: logic_error );
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
```

A note on the discontinuity at the 180 meridian. For bounding boxes that have left border specified as a greater longitude than the right border (e.g. 170 to -170), the right border will be adjusted to the 180..540 area using formula 360 + value (in the example, 190 = 360 + (-170)).

Corresponding logic will have to be implemented in the geo-filtering and distance calculation code.

The setters will allow latitudes with the same adjustment preformed externally, i.e. with the right border > 180 and the left border between ( right - 360 ) and right.

Other attempts to set latitudes outside of -180.. 180 will result in an exception thrown by the methods of class BoundingBox. Same for longitudes outside of -90..90.   

### API methods

```c++
class Dataset {

public:
	Dataset ( const char* filename );

	Response Search(const char* text, 
			        Format format = { DefaultResults });
			
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

    // The reference is valid until the next call to Place()
    // Use Feature::Clone() to make a caller-owned copy
	const GeocodeJSON* :: Feature& Place ( Id ) const throw ( logic_error );
};
```

### GeocodeJSON objects

```c++
namespace GeocodeJSON {

	/**
	* GeocodeJSON Geometry. For now, only Point in Geographic Coordinate Reference System
	*/
	class Geometry
	{	//  
	public:
		Coordinate Latitude() const;
		Coordinate Longitude() const;
	};

	/**
	* GeocodeJSON Feature object corresponding to a single Location
	*/
	class Feature
	{
	public:
        // generic access to searchable properties
        typedef enum 
        {
            Property_begin = 0,
            
            Property_Label = Property_begin,
            Property_Name,
            Property_HouseNumber,
            Property_Street,
            Property_Postcode,
            Property_City,
            Property_District,
            Property_County,
            Property_Region,
            Property_RegionAbbr,
            Property_Country,
            Property_CountryAbbr,
            Property_Geohash,
            
            Property_end
        } SearchablePropertyId;
        
    public:
        virtual ~Feature() = 0;
        
	public:
        virtual const BoundingBox* GetBoundingBox() const = 0; // optional
		virtual const Geometry* GetGeometry() const = 0;

		// Access to Feature object's properties, based on 
        // https://github.com/pelias/geocodejson-spec/tree/master/draft
        // optional properties return 0/nullptr if omitted
        
		virtual const char* Id () const = 0;
		virtual const char* Layer () const = 0;
        
		virtual const char* Source () const = 0; // optional
		virtual uint32_t AccuracyMeters () const = 0; // optional
		virtual double Confidence () const = 0; // 0.0 .. 1.0; optional
        
		virtual const char* Label () const = 0; 
		virtual const char* Name () const = 0; // "recommended" but still optional
        
		virtual const char* HouseNumber () const = 0; // optional
		virtual const char* Street () const = 0; // optional
		virtual const char* Postcode () const = 0; // optional
		virtual const char* City () const = 0; // optional
		virtual const char* District () const = 0; // optional
		virtual const char* County () const = 0; // optional
		virtual const char* Region () const = 0; // optional
		virtual const char* RegionAbbr () const = 0; // optional
		virtual const char* Country () const = 0; // optional
		virtual const char* CountryAbbr () const = 0; // optional
        
		virtual const char* Admin ( unsigned int level ) const = 0; // optional; level from 1 to 10
        
		virtual const char* Geohash () const = 0; // optional
        
        const char* SearchableProperty ( SearchablePropertyId ) const 
            throw ( std :: invalid_argument );
        
        virtual std::string Stringify() const = 0;
        
        // create a copy of this object, to be deleted by the caller
        virtual Feature* Clone() const = 0;
	};

}
```

A note on handling Unicode: UTF-8 on input (the JSON reader must support; RapidJSON does); internally using UTF-8 encoded byte strings. 

##Supporting interfaces and algorithms

It is important to note that read time containers are (at least conceptually) immutable, so they are best represented as a buffer loaded into memory without any additional processing.
Write side can analyze the structure and choose alternative representations or suggest more efficient lookup methods.

The usefulness of naive implementations is in their simplicity, allowing for a rapid development of a functionally complete system. The quickly-built system then can be used to explore functionality issues, as well as investigated for performance bottlenecks. The feedback from performance study will drive replacement of naive designs with more performant ones. Care must be taken to insulate the rest of the systems from the details of naive/efficient designs, which is achieved through the use of interfaces.

Since the implementation language is C++, interfaces are modeled by abstract classes. Alternatively, use Pimpl idiom to hide implementation details.

###Data:
**Data**: a sequence of GeocodeJSON-derived objects.
A naive implementation would use an STL vector of in-memory GeocodeJSON::Feature objects.

```c++
class Data
{
public:
    // throws invalid_argument if Id is out of range
	virtual const GeocodeJSON::Feature& Get ( Id ) const throw ( std :: invalid_argument ) = 0;
	virtual size_t Count () const = 0;
};
```

A more memory-efficient representation would store serialized objects in a byte buffer (in memory or on disk) and involve an index:

**Index**: an array of offsets into data buffer; subscript is the object Id. Size of an Id value depends on the total size of the **Data**; 4 bytes should be enough but consider packing into less than 32 bits.

```c++
class Index
{
public:
	typedef size_t Offset;

public:
	virtual Offset Get ( Id ) const = 0;
};   
```

The memory efficiency comes at the cost of reduced speed of access to individual objects, which can be addressed by adding a cache of inflated Location objects.

The byte buffer itself, if it resides on disk, can be split into blocks and cached.  

GeocodeJSON::Feature objects returned by the Data interface do not have to be fully inflated. Since in many cases only selected properties of a given object will be of interest to the calling code, it may be beneficial to implement lazy access to properties. As another option, Location objects and/or their properties can be represented by references to corresponding portions of the data buffer. With that in mind, GeocodeJSON::Feature should be fully abstract or at least opaque (using Pimpl pattern).

```c++
class Feature
{	// This is a GeocodeJSON Feature object corresponding to a single Location
public:
	const GeometryObject* Geometry() const = 0;
	// ... etc - see API Operations
}
```

or, using the Pimpl idiom,

```c++
class Feature
{	// This is a GeocodeJSON Feature object corresponding to a single Location
private:
	class Impl;	// defined in the implementation file
	Impl* m_pimpl;

public:
	const GeometryObject* Geometry() const;
	// ... etc - see API Operations
}
```

###Bounding Box search support

To support search filtered by a BoundingBox, we need an ability to filter locations. Two different cases involve locations specified by a single point (a LatLon value) and by a geoshape ( an open or closed polygon ).

**PointLatIndex**: an array of pairs "coordinate, object Id" sorted by coordinate (latitude). This index will only contain objects with location specified as a point. There may be multiple entries with matching coordinates.

The principal operation will be to extract the subset of points located between 2 coordinates [*low* .. *high*], represented as an iterator:

	FindSegment ( CoordIndex, LowCoord, HighCoord ) : ( firstIdx, lastIdx )
		firstIdx = FindFirstGreaterThanOrEqual ( CoordIndex, 0, size ( CoordIndex ), LowCoord )  
		lastIdx = FindLastLessThanOrEqual ( CoordIndex,
											firstIdx,
											size ( CoordIndex ),
											HighCoord )  
		return Iterator ( firstIdx, lastIdx )

The coordinate will be initially represented as a double. The alternatives would be float and integer-based fixed point; it may make sense to decrease precision of the coordinate to save space in this structure. The full-precision coordinates extracted from the objects will then be used at the scoring stage to filter out false hits, if necessary.

For additional memory savings at the cost of lookup speed, pairs "coordinate, object Id" may be packed into fewer than 12 bytes (12 = 8 for double + 4 for Id). The minimum number of bits required to represent all entries can be calculated at the construction time and saved with the structure.  

The low level design should allow easy switching between representations. Some experimentation will be required to estimate memory/speed tradeoffs between different representations.

A simplistic implementation of the index may use STL multimap template. 

**PointLonIndex**: a similar structure using longitude as the coordinate.

####LatLon search support
For simplicity, we will represent a circular area specified as ( **LatLon**, R ) with a square ( left = Lon-R/2, top = Lat+R/2, right = Lon+R/2, bottom = Lat-R/2 ). This expands the area by about 20% but allows to use the same fast location filtering as for bounding boxes. The potential extraneous hits will have distance to the center of the circle greater than R and can be filtered out later, possibly as late as the scoring stage.

Note that LatLon search with the default radius covers the entire dataset. Filtering in this case can be bypassed, or the corresponding filter can be represented with a bitmap with all bits set to 1.   

####Locations with point geometry
Initially, all locations in the dataset will be represented as a single point ( centroid for lines and polygons, see [https://en.wikipedia.org/wiki/Centroid](https://en.wikipedia.org/wiki/Centroid ).

The result of the search is converted into a **Location Filter**: a bitmap indexed by object Id with 1 denoting presence of object in the index.

```c++
class LocationFilter //naive: : public vector < bool >
{
public:
	LocationFilter ( size_t count ); // reserve and 0-initialize
	LocationFilter& and ( const LocationFilter& );
};  
```

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

The distance between two points will be calculated using the Pythagorean theorem ( source: http://www.movable-type.co.uk/scripts/latlong.html, lat/lon values are in radians ):

	Distance ( LatLon ( lat1, lon1 ), LatLon ( lat2, lon2 ) ) =
		6,371km * sqrt ( ( ( lon2 - lon1 ) * cos ( lat1 + lat2 ) ) ** 2 + ( lat2 - lat1 ) ** 2 )  

Note that the system-provided implementation of sqrt and cos are relatively slow and can become a performance bottleneck. If that happens, be prepared to replace them with less accurate but faster versions, e.g. based on using Taylor series with a low number of iterations (see https://en.wikipedia.org/wiki/Taylor_series ).

As an option, we can consider implementing more computationally expensive but more accurate Haversine formula [https://en.wikipedia.org/wiki/Haversine_formula](https://en.wikipedia.org/wiki/Haversine_formula). 

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

The index has two APIs, one for the read side and one for the write side. Each side will be represented by its own class, with the two classes possibly sharing a common representation hidden by the interface.

The write side is used while populating the dataset, either on the server (in which case the index needs to support serialization for transmission), or on the client in the process of converting JSON to Pelopia's own (in-memory or on-disk) formal.

The interfaces:

```c++
class TextIndexWriter
{    
public:
	virtual AddTermUse ( const string& term, 
	                     Id object, 
	                     GeocodeJSON :: PropertyId, 
	                     size_t offset, 
	                     size_t length ) = 0;
};

class TextIndexReader
{   
public:
	class TermUse   
	{
	public:
		virtual Id ObjectId () const = 0;
		virtual GeocodeJSON :: PropertyId PropertyId () const = 0;
		virtual void Position ( size_t& index, size_t& offset, size_t& length ) const = 0;
	};

	class Payload // naive: vector < TermUse >
	{	// the collection is sorted by increasing object Id, then by increasing index, then by increasing position  
		virtual size_t Count () const = 0;
        virtual size_t FeatureCount () const = 0; // # of distinct feature Ids
		virtual const TermUse& GetTermUse ( size_t index ) const throw (index_out_of_bounds) = 0;
	};

	class Node
	{   
	public:
		virtual const Payload& GetPayload () const = 0;

		virtual size_t ChildrenCount () const = 0;
		virtual const Node& GetChild ( size_t index ) const throw (index_out_of_bounds) = 0;
	};

public:
	virtual const Node* Locate ( const string& prefix ) const = 0;
};

```

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

####Text scoring

NOTE. Currently, we use the "Label" property of GeocodeJSON object to represent its full name and address. The text of the property should be normalized before it is used for text scoring. 

The calculation of TEXTSCORE is done for every GeocodeJSON object (feature) that passes filtering:

For each term in the *normalized* full text query, calculate its WEIGHT:

Term Frequency:

	TF = sqrt ( # of appearances of the term in the *Label* field of the feature )

Inverse document frequency: the more often the term appears in the dataset, the less relevant it is. This value can be pre-calculated for every term in the dataset.

	IDF = 1 + log ( totalFeatures / ( 1 + # of features containing the term ) )   


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

	TEXTSCORE = QUERY_NORM * COORD * sum ( WEIGHT,  for each term in query )

In addition, boost score of fields where the order of found terms matches the order of terms in the query. Details **TBD**

###Reverse search support
**TBD**

####Geo scoring
The purpose of the geo-scoring stage is twofold.

First, the coordinates of retrieved locations are used to filter out false hits appearing due to representation of circles and polygons with an enclosing bounding box, both on the (circular) search region's and on the (shaped) location's side.

Second, geographical distance between the location and the center of the search region is used as a tie breaker for elements of the result set with matching scores. For a shaped location (when supported), it can be the distance between the center of the search region and the location's centroid. Alternatively, and more expensively calculation-wise, it can be the distance between the center of the search region and the location's nearest point to it.

Our first implementation of geo scorer will use *exp* decay function curve with the following parameters (see https://www.elastic.co/guide/en/elasticsearch/guide/current/decay-functions.html for an explanation of use):
- origin: fixed at 0 (0 distance will always result in score of 1)
- decay: fixed at 0.5 (score at distance *scale* - see below)
- scale: distance that will result in score *decay*. Make it a fraction of X, where X is the distance between the dataset's farthest points (the latter should be pre-calculated; we can use the length of the diagonal of the dataset's bounding box, if available. Alternatively, can use the diagonal of the bounding box of the search)
- offset: fixed at 0. It may also make sense to make it a fraction of X, say S = 1%. In this case, *scale* will be set to ( X - S / 2 ) / 2

The formula for geo score using exp decay is

    GEOSCORE_EXP = exp ( Lambda * max ( 0, DistanceFromCenter - offset ) )
    
    Lambda = log ( Decay ) / Scale

####Scoring API

```c++
class Scorer
{
public:
	virtual MatchQuality Score ( Id feature ) const = 0;
	
protected:
    Scorer ( const Dataset& );
}
```	

The text scorer will accept the original normalized query:

```c++
class TextScorer : public Scorer
{
public:
    TextScorer ( const Dataset &, const Normalizer :: Result & query );
	virtual MatchQuality Score ( Id feature ) const;
}
```	

The geo scorer will accept additional information on the geography of dataset (i.e. its boundaries) in order to involve distance in the scoring function:

```c++
class GeoScorer : public Scorer
{
public:
    GeoScorer ( const Dataset &, const LatLon & center ); 
	virtual MatchQuality Score ( Id feature ) const;
}
```	

Scores returned by the two scorer objects will be multiplied (possibly weighed based on configuration-like parameters in Dataset) to produce the final MatchQuality used to sort entries in the Reponse. 

###Dataset
Originally, dataset will be represented as a plain JSON file, which will be parsed and converted into library's internal structures at the Dataset object initialization time. For large datasets that is likely to become prohibitively slow, at which point we will have to introduce an intermediate representation (**IR**) used to store dataset in a more ready-to-consume (and external memory-efficient) format than JSON.

Conversion of JSON datasets into their **IR**s will ideally happen on a (MapZen) server, so that the dataset arrives to the user's device ready for use without further transformation. Alternatively, transformation can take place on the user's device post-download, via a utility program.

To account for these alternatives, the transformation code has to be decoupled from the search portion of the library, so that it can packaged for autonomous use on server or post-download.

Note that address normalization is a part of converting dataset from MapZen-created GeocodeJSON into Pelopia's internal structures.

To reduce the footprint on the user's device and, in the case of server-side transformation, the size of transmission, the **IR** should involve compression.

The library should provide some degree of validation of the dataset it is offered.

**IR** has to be versioned. Backward compatibility with older versions of the **IR** should be preserved.

##Analysis and transformation
Examine libpostal for its usefulness in (international) postal address normalization. Evaluate resource requirements and quality.
Try to use the fact that normally the native language of the map is determined by the dataset's region.

For naive implementation, create a simplistic tokenizer akin to [https://www.elastic.co/guide/en/elasticsearch/reference/current/analysis-edgengram-tokenizer.html](https://www.elastic.co/guide/en/elasticsearch/reference/current/analysis-edgengram-tokenizer.html) .

The same address normalization logic has to be applied to addresses as part of the dataset preparation process and to queries before search.

The normalizer API is designed to minmize allocations and copying: 

```c++
class Normalizer
{
public:
    typedef struct {
        size_t startBytes;      // original term, offset from the start of the phrase
        size_t lengthBytes;     // original term, size up to and excluding the following punctuation/white space
        const char32_t* norm;   // NUL-terminated normalized term in 32-bit Unicode
    } NormalizedTerm;
    
    typedef std::vector < NormalizedTerm > Result;
    
public:
    virtual ~Normalizer() {}
    
    // phrase is in UTF-8-encoded Unicode
    // the returned reference stays valid until the next call to Normalize
    virtual const Result& Normalize ( const char* phrase ) = 0;
}
```	

A naive normalizer will break the given phrase into individual terms based on punctuation and white space. To detect punctuation and white space characters, use ::ispunct() and ::isspace() from header \<cctype>.

For a little more advanced normalizer, consider using locale-specific definitons of ispunct and isspace from header \<locale>; the specific locale to be used would be specified by the client code, or determined based on the dataset's contents.

For dealing with UTF-8 encoding, use the UTF8-CPP library (http://utfcpp.sourceforge.net/)

##Exception Handling

Methods of the API will throw exceptions to report errors. The exceptions generated by the library will be belong to classes derived from standard exception classes declared in \<stdexcept>, for example:

```c++
#include <stdexcept>
namespace Pelopia
{
    class NotImplementedError : public std :: logic_error
    {
    };
}

```

There will be a an exception handling wrapper which mirrors the original API in an exception-free manner. All methods will have specifier ```throw()```.
Each method will consist of a call to the corresponding method from the original API in a try/catch clause. Use separate catch clauses for Pelopia-originated exceptions. Use catch-all for all other exceptions. Convert exceptions into an enumeration of error codes. Return error code from each wrapper method. If the original method returns a value, make it an output parameter (the last).  

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
For the purposes of this project, only reading of GeocodeJSON files is required. 

A typical JSON library support two modes of reading a JSON file. 

One mode (DOM, Document Oriented Model) would be to read in the entire document and provide random access to the objects contained therein. This is easier to implement and will work for naive implementations or smaller files, at the cost of higher memory consumption. 

Another mode (Stream), which may be the only option for bigger files/production, would parse the input file object-by-object and return them to calling code for processing one by one.


The distinction between the two modes will be hidden behind an interface built on the second mode. Internally, naive implementation may use the first mode. 

```c++
namespace GeocodeJSON {
    
    class Reader
    {
    public:
        virtual ~Reader () = 0;
        
        // returns 0 at the end of the input
        // calls past the end the input will throw
        // any exceptions thrown by the underlying stream will be propagated
        // objects not conforming to JSON syntax will throw 
        // objects not conforming to GeocodeJSON will throw 
        virtual const Feature* NextValue () throw ( logic_error ) = 0; 
    }
}    
```

The objects returned by NextValue are owned by the Reader class. Each returned pointer is guaranteed to be valid until the next call to NextValue() or destruction of the Reader object, so the client code would have to call GeocodeJSON::Clone() if it needs to hold on to the object any longer than that. 

The  naive implementation will use the DOM mode, hold the entire GeocodeJSON value in memory, and provide consecutive read-only access to its Feature sub-elements.

The optimized implementation will use the stream mode, with Feature elements parsed one at a time.

Both modes will be implemented using RapidJSON, as separate subclasses of Reader, GeocodeJSON::Reader_Rapid_DOM and GeocodeJSON::Reader_Rapid_Stream.

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
		Python/     - Python examples
		...			- other languages
	inc/
		pelopia/	- externally visible Pelopia headers
		test/       - Catch header
		... 		- other 3d party headers, as needed
	src/
        test/		- unit tests
		rapidjson/  - RapidJSON sources
		*.cpp 		- library code
		*.h 		- library-internal headers
    acceptance/		- acceptance tests
    language/       - language bindings/wrappers
        C/
        python/
        ...

##Examples

###C++

Using Search:

```c++
	#include <iostream>

	#include <pelopia/Dataset.h>
	#include <pelopia/GeocodeJSON.h>

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
			GeocodeJSON::Feature place = ds.Place(id);
			cout << (i + 1) << " ( " << score << " ) "
				<< ": " << place.Name() << ", " << place.Label()
				<< endl;
		}
	}
```

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
Develop an acceptance test runner akin to Pelias's on one platform (Linux).

For acceptance testing, capture some results from Pelias searches and, making sure the location criteria match, use them as expected results. Be prepared to update the expected results as Pelias evolves. 

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

|ID|Priority|Title|
|---|---|---|
| ___1___ | | ___Various___ |
| 1.1 | 1 | Logging to a file
| 1.2 | 1 | Exception handling wrapper
| 1.3 | 2 | Use a dataset from Mapzen-produced JSON
| 1.4 | 3 | Logging to a client-supplied stream
| ___2___ |  | ___Search, Naive___ |  
| 2.1 | 0 | Search using one term text, naive implementation
| 2.2 | 0 | Search using one term text using focus LatLon, naive implementation
| 2.3 | 1 | Search for administrative area (city, state, etc.)
| 2.4 | 1 | Search using one incomplete term text, naive implementation
| 2.5 | 1 | Search using multiple complete terms and one incomplete term text, naive implementation
| 2.6 | 2 | Search using one term text using focus BoundingBox, naive implementation
| 2.7 | 2 | Search for address
| 2.8 | 2 | Search using multiple term text, naive implementation
| 2.9 | 2 | Autocomplete, naive
| 2.10 | 3 | Reverse search, naive
| ___3___  |  | ___Data___ |
| 3.1 | 2 | Use a dataset locally converted from Mapzen-produced JSON
| 3.2 | 3 | Use a dataset downloaded from a Pelopia-specific Mapzen endpoint in Pelopia format
| ___4___ |  | ___Search, Optimized___ |   
| 4.1 | 3 | Search using one term text using BoundingBox, optimized implementation
| 4.2 | 3 | Search using one term text using LatLon, optimized implementation
| 4.3 | 3 | Search using multiple term text, optimized implementation
| 4.4 | 3 | Search using one incomplete term text, optimized implementation
| 4.5 | 3 | Search using multiple complete term and one incomplete term text, optimized implementation
| 4.6 | 3 | Autocomplete, optimized
| 4.7 | 3 | Reverse search, optimized
| ___5___ |  | ___APIs___  |
| 5.1 | 0 | Search through a C++ API
| 5.2 | 3 | Search through a Swift/Objective C API
| 5.3 | 3 | Search through a C API
| 5.4 | 3 | Search through a Java API
| 5.5 | 3 | Search through a Python API
| 5.6 | 3 | Search through a Node.js API
| ___6___ |  | ___Build___ |
| 6.1 | 0 | Download/build from sources in a clean environment on Linux
| 6.2 | 0 | Download/build from sources in a clean environment on OSX
| 6.3 | 1 | Convert build system to CMake
| 6.4 | 2 | Download/build from sources in a clean environment on iOS
| 6.5 | 3 | Download/build from sources in a clean environment on Windows
| 6.6 | 3 | Download/build from sources in a clean environment on Android
| ___7___ |  | ___Install___ |
| 7.1 | 3 | Installation package for Windows
| 7.2 | 3 | Installation package for Linux
| 7.3 | 3 | Installation package for Linux, root mode
| 7.4 | 3 | Installation package for Android
| 7.5 | 3 | Installation package for OSX
| 7.6 | 3 | Installation package for iOS

##Milestones

1. Text only search works
1. Search for address works
1. Search with LatLon works
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
