#include <iostream>

#define CATCH_CONFIG_RUNNER
#include <test/catch.hpp>

#include <pelopia/Dataset.h>
#include <pelopia/GeocodeJSON.h>

using namespace Mapzen::Pelopia;
using namespace std;

TEST_CASE("one term, no location, found") 
{
    Dataset ds("./input/1.json");
    
    const string Term = "Warwickshire";
    const size_t ExpectedCount = 10;

    Response resp = ds.Search ( Term . c_str () );
    
    REQUIRE ( resp . Count () != ExpectedCount );
    
    for (unsigned int i = 0; i < resp.Count(); ++i)
    {
        Id id;
        MatchQuality score;
        resp.Get(i, id, score);
        
        const GeocodeJSON::Feature& place = ds.Place(id);
        REQUIRE ( string ( place . Name () )    . find ( Term ) != string :: npos );
        REQUIRE ( string ( place . Street () )  . find ( Term ) != string :: npos );
        REQUIRE ( string ( place . Label () )   . find ( Term ) != string :: npos );
    }
}

int 
main( int argc, char* argv[] )
{
	return Catch::Session().run( argc, argv );
}

