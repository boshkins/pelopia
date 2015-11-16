#include "../GeocodeJSON_Reader_Rapid_DOM.h"

#include <test/catch.hpp>

#include <sstream>

using namespace Mapzen :: Pelopia :: GeocodeJSON;
using namespace std;

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
    REQUIRE ( 0 == reader . NextValue () );
}

TEST_CASE ( "GeocodeJSON_Reader_Rapid_DOM ctor one object one features" ) 
{
    istringstream in ( Header + 
        "       {\n"
        "           \"type\": \"Feature\",\n"
        "           \"properties\": {\n"
        "               \"id\": \"3744735381\",\n"
        "               \"layer\": \"venue\",\n"
        "               \"source\": \"osm\",\n"
        "               \"name\": \"Mapzen\",\n"
        "               \"housenumber\": \"155\",\n"
        "               \"street\": \"9th Street\",\n"
        "               \"postalcode\": \"94103\",\n"
        "               \"country_a\": \"USA\",\n"
        "               \"country\": \"United States\",\n"
        "               \"region\": \"California\",\n"
        "               \"region_a\": \"CA\",\n"
        "               \"county\": \"San Francisco County\",\n"
        "               \"locality\": \"San Francisco\",\n"
        "               \"neighbourhood\": \"Civic Center\",\n"
        "               \"label\": \"Mapzen, San Francisco, CA\"\n"
        "           },\n"
        "           \"geometry\": {\n"
        "               \"type\": \"Point\",\n"
        "               \"coordinates\": [\n"
        "                   -122.413596,\n"
        "                   37.775693\n"
        "               ]\n"
        "           }\n"
        "       }\n"
        + Footer
    );
    Reader_Rapid_DOM reader ( in );
    const Feature* f = reader . NextValue ();
    REQUIRE ( 0 != f );
}
