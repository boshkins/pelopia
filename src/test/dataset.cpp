#include <pelopia/Dataset.h>

#include <test/catch.hpp>

#include <stdexcept>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "Dataset Construct from a non-existent file" ) 
{
    REQUIRE_THROWS_AS ( Dataset ds ( "./input/notthere" ), logic_error );
}

TEST_CASE ( "Dataset Construct from a bad JSON file" ) 
{
    REQUIRE_THROWS_AS ( Dataset ds ( "./input/invalid.json" ), logic_error );
}

TEST_CASE ( "Search one term, all defaults, not found" ) 
{
    Dataset ds ( "./input/one-term.json" );
    const Response& resp = ds.Search("Notthereshire");
    REQUIRE ( 0 == resp.Count() );
}

TEST_CASE ( "Search one term, all defaults, found" ) 
{
    Dataset ds ( "./input/one-term.json" );
    const Response& resp = ds.Search("Warwickshire");
    REQUIRE ( 0 != resp.Count() );
}

