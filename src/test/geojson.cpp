#include "../GeocodeJSON_Reader_Rapid_DOM.h"

#include <test/catch.hpp>

#include <sstream>

#include <pelopia/BoundingBox.h>

using namespace Mapzen :: Pelopia :: GeocodeJSON;
using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "GeocodeJSON::Geometry" )
{
    Geometry g ( 1.0, 2.0 );
    
    SECTION ( "ctor" )
    {
        REQUIRE ( 1.0 == g . Latitude () );
        REQUIRE ( 2.0 == g . Longitude () );
    }
    SECTION ( "equality")
    {
        REQUIRE ( Geometry ( 1.0, 2.0 ) == g );
        REQUIRE ( Geometry ( 2.0, 1.0 ) != g );
    }
    SECTION ( "setters" )
    {
        g . SetLatitude ( 3.0 );
        g . SetLongitude ( 4.0 );
        REQUIRE ( Geometry ( 3.0, 4.0 ) == g );
    }
}

TEST_CASE ( "GeocodeJSON::Geometry default ctor" )
{
    Geometry g;
    REQUIRE ( Geometry ( 0, 0 ) == g );
}


TEST_CASE ( "GeocodeJSON_Reader_Rapid_DOM ctor empty stream" ) 
{
    istringstream in;
    REQUIRE_THROWS ( Reader_Rapid_DOM reader ( in ) );
}

static const string Header = 
    "{\n"
    "   \"geocoding\": {\n"
    "       \"version\": \"0.1\",\n"
    "       \"attribution\": \"http://pelias.mapzen.com/v1/attribution\",\n"
    "       \"query\": {\n"
    "           \"text\": \"Mapzen\",\n"
    "           \"parsed_text\": {\n"
    "               \"regions\": [\n"
    "                   \"Mapzen\"\n"
    "               ]\n"
    "           },\n"
    "           \"size\": 10,\n"
    "           \"private\": false\n"
    "       },\n"
    "       \"engine\": {\n"
    "           \"name\": \"Pelias\",\n"
    "           \"author\": \"Mapzen\",\n"
    "           \"version\": \"1.0\"\n"
    "       },\n"
    "       \"timestamp\": 1442882160591\n"
    "   },\n"
    "   \"type\": \"FeatureCollection\",\n"
    "   \"features\": [\n";

static const string Footer = 
    "  ]\n"
    "}\n";

TEST_CASE ( "GeocodeJSON_Reader_Rapid_DOM ctor one object no features" ) 
{
    istringstream in ( Header + Footer );
    Reader_Rapid_DOM reader ( in );
    REQUIRE ( nullptr == reader . NextValue () );
}

static const string FeatureComplete = /* all properties present */
        "       {\n"
        "           \"type\": \"Feature\",\n"
        "           \"properties\": {\n"
        "               \"id\": \"3744735381\",\n"
        "               \"layer\": \"venue\",\n"
        "               \"source\": \"osm\",\n"
        "               \"accuracy\": 20,\n"
        "               \"confidence\": 0.8,\n"
        "               \"label\": \"Mapzen, San Francisco, CA\",\n"
        "               \"name\": \"Mapzen\",\n"
        "               \"housenumber\": \"155\",\n"
        "               \"street\": \"9th Street\",\n"
        "               \"postcode\": \"94103\",\n"
        "               \"city\": \"San Francisco\",\n"
        "               \"district\": \"Civic Center\",\n"
        "               \"county\": \"San Francisco County\",\n"
        "               \"region\": \"California\",\n"
        "               \"region_abbr\": \"CA\",\n"
        "               \"country\": \"United States\",\n"
        "               \"country_abbr\": \"USA\",\n"
        "               \"admin\": {\n"
        "                   \"level1\": \"admin level 1\",\n"
        "                   \"level2\": \"admin level 2\",\n"
        "                   \"level3\": \"admin level 3\",\n"
        "                   \"level4\": \"admin level 4\",\n"
        "                   \"level5\": \"admin level 5\",\n"
        "                   \"level6\": \"admin level 6\",\n"
        "                   \"level7\": \"admin level 7\",\n"
        "                   \"level8\": \"admin level 8\",\n"
        "                   \"level9\": \"admin level 9\",\n"
        "                   \"level10\": \"admin level 10\"\n"
        "               },\n"
        "               \"geohash\" : \"Ehugh5oofiToh9aWe3\"\n"
        "           },\n"
        "           \"bbox\": [-1.2, -3.4, 5.6, 7.8],\n"
        "           \"geometry\": {\n"
        "               \"type\": \"Point\",\n"
        "               \"coordinates\": [\n"
        "                   -122.413596,\n"
        "                   37.775693\n"
        "               ]\n"
        "           }\n"
        "       }\n"
;
        
TEST_CASE ( "GeocodeJSON_Reader_Rapid_DOM ctor one object one feature" ) 
{
    istringstream in ( Header + FeatureComplete + Footer );
    Reader_Rapid_DOM reader ( in );
    Feature* f = reader . NextValue ();
    REQUIRE ( nullptr != f );
    
    SECTION ( "GetGeometry" ) 
    {
        const Geometry* g = f -> GetGeometry();
        REQUIRE ( nullptr != g );
        REQUIRE ( -122.413596   == g -> Longitude () );
        REQUIRE ( 37.775693     == g -> Latitude () );
    }
    SECTION ( "Id" )            { REQUIRE ( string ( "3744735381" )                 == f -> Id () ); }
    SECTION ( "layer" )         { REQUIRE ( string ( "venue" )                      == f -> Layer () ); }
    SECTION ( "source" )        { REQUIRE ( string ( "osm" )                        == f -> Source () ); }
    SECTION ( "accuracy" )      { REQUIRE ( 20u                                     == f -> AccuracyMeters () ); }
    SECTION ( "confidence" )    { REQUIRE ( 0.8                                     == f -> Confidence () ); }
    SECTION ( "label" )         { REQUIRE ( string ( "Mapzen, San Francisco, CA" )  == f -> Label () ); }
    SECTION ( "name" )          { REQUIRE ( string ( "Mapzen" )                     == f -> Name () ); }
    SECTION ( "housenumber" )   { REQUIRE ( string ( "155" )                        == f -> HouseNumber () ); }
    SECTION ( "street" )        { REQUIRE ( string ( "9th Street" )                 == f -> Street () ); }
    SECTION ( "postcode" )      { REQUIRE ( string ( "94103" )                      == f -> Postcode () ); }
    SECTION ( "city" )          { REQUIRE ( string ( "San Francisco" )              == f -> City () ); }
    SECTION ( "district" )      { REQUIRE ( string ( "Civic Center" )               == f -> District () ); }
    SECTION ( "county" )        { REQUIRE ( string ( "San Francisco County" )       == f -> County () ); }
    SECTION ( "region" )        { REQUIRE ( string ( "California" )                 == f -> Region () ); }
    SECTION ( "regionabbr" )    { REQUIRE ( string ( "CA" )                         == f -> RegionAbbr () ); }
    SECTION ( "country" )       { REQUIRE ( string ( "United States" )              == f -> Country () ); }
    SECTION ( "country_abbr" )  { REQUIRE ( string ( "USA" )                        == f -> CountryAbbr () ); }
    SECTION ( "admin[1]" )      { REQUIRE ( string ( "admin level 1" )              == f -> Admin ( 1 ) ); }
    SECTION ( "admin[7]" )      { REQUIRE ( string ( "admin level 7" )              == f -> Admin ( 7 ) ); }
    SECTION ( "geohash" )       { REQUIRE ( string ( "Ehugh5oofiToh9aWe3" )         == f -> Geohash () ); }
    SECTION ( "bbox" )          
    { 
        const BoundingBox* bb = f -> GetBoundingBox ();
        REQUIRE ( nullptr != bb );
        REQUIRE ( BoundingBox ( 7.8, -1.2, -3.4, 5.6 ) == *bb ); 
    }
    
    delete f;
}

static const string OptionalAbsent = /* mandatory properties only */
        "       {\n"
        "           \"type\": \"Feature\",\n"
        "           \"properties\": {\n"
        "               \"id\": \"3744735381\",\n"
        "               \"layer\": \"venue\",\n"
        "               \"region_abbr\": \"CA\"\n"
        "           },\n"
        "           \"geometry\": {\n"
        "               \"type\": \"Point\",\n"
        "               \"coordinates\": [\n"
        "                   -122.413596,\n"
        "                   37.775693\n"
        "               ]\n"
        "           }\n"
        "       }\n"
;

TEST_CASE ( "GeocodeJSON_Reader_Rapid_DOM ctor one object no optional properties" ) 
{
    istringstream in ( Header + OptionalAbsent + Footer );
    Reader_Rapid_DOM reader ( in );
    const Feature* f = reader . NextValue ();
    REQUIRE ( nullptr != f );
    
    SECTION ( "GetGeometry" ) 
    {
        const Geometry* g = f -> GetGeometry();
        REQUIRE ( nullptr != g );
        REQUIRE ( -122.413596   == g -> Longitude () );
        REQUIRE ( 37.775693     == g -> Latitude () );
    }
    SECTION ( "Id" )            { REQUIRE ( string ( "3744735381" ) == f -> Id () ); }
    SECTION ( "layer" )         { REQUIRE ( string ( "venue" )      == f -> Layer () ); }
    SECTION ( "source" )        { REQUIRE ( nullptr                 == f -> Source () ); }
    SECTION ( "accuracy" )      { REQUIRE ( 0                       == f -> AccuracyMeters () ); }
    SECTION ( "confidence" )    { REQUIRE ( 0                       == f -> Confidence () ); }
    SECTION ( "label" )         { REQUIRE ( nullptr                 == f -> Label () ); }
    SECTION ( "name" )          { REQUIRE ( nullptr                 == f -> Name () ); }
    SECTION ( "housenumber" )   { REQUIRE ( nullptr                 == f -> HouseNumber () ); }
    SECTION ( "street" )        { REQUIRE ( nullptr                 == f -> Street () ); }
    SECTION ( "postcode" )      { REQUIRE ( nullptr                 == f -> Postcode () ); }
    SECTION ( "city" )          { REQUIRE ( nullptr                 == f -> City () ); }
    SECTION ( "district" )      { REQUIRE ( nullptr                 == f -> District () ); }
    SECTION ( "county" )        { REQUIRE ( nullptr                 == f -> County () ); }
    SECTION ( "region" )        { REQUIRE ( nullptr                 == f -> Region () ); }
    SECTION ( "regionabbr" )    { REQUIRE ( string ( "CA" )         == f -> RegionAbbr () ); }
    SECTION ( "country" )       { REQUIRE ( nullptr                 == f -> Country () ); }
    SECTION ( "country_abbr" )  { REQUIRE ( nullptr                 == f -> CountryAbbr () ); }
    SECTION ( "admin[1]" )      { REQUIRE ( nullptr                 == f -> Admin ( 1 ) ); }
    SECTION ( "admin[7]" )      { REQUIRE ( nullptr                 == f -> Admin ( 7 ) ); }
    SECTION ( "geohash" )       { REQUIRE ( nullptr                 == f -> Geohash () ); }
    SECTION ( "bbox" )          { REQUIRE ( nullptr                 == f -> GetBoundingBox () ); }
    
    delete f;
}

