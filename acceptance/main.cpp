#include <iostream>

#define CATCH_CONFIG_RUNNER
#include <test/catch.hpp>

#include <pelopia/Dataset.h>
#include <pelopia/GeocodeJSON.h>

using namespace Mapzen::Pelopia;
using namespace std;

TEST_CASE("one term, no location") 
{
    Dataset ds("./input/1.json");
    
    SECTION ("found 10 or more")
    {
        const string Term = "Warwickshire";
        const size_t ExpectedCount = 10;

        const Response& resp = ds.Search ( Term . c_str () );
        
        REQUIRE ( DefaultResults == resp . Count () );
        
        for (unsigned int i = 0; i < resp.Count(); ++i)
        {
            Id id = resp.Get(i);
            const GeocodeJSON::Feature& place = ds.Place(id);
            REQUIRE ( string ( place . Label () ) . find ( Term ) != string :: npos );
        }
    }
    SECTION ( "found less than 10" ) 
    {
        const string Term = "Bermuda";

        const Response& resp = ds.Search ( Term . c_str () );
        
        REQUIRE ( 6 == resp . Count () );
        
        for (unsigned int i = 0; i < resp.Count(); ++i)
        {
            Id id = resp.Get(i);
            const GeocodeJSON::Feature& place = ds.Place(id);
            REQUIRE ( string ( place . Label () ) . find ( Term ) != string :: npos );
        }
    }

    SECTION ( "found none" ) 
    {
        const Response& resp = ds.Search ( "Salisbury" );
        REQUIRE ( 0 == resp . Count () );
    }
}    

int 
main( int argc, char* argv[] )
{
	return Catch::Session().run( argc, argv );
}

