#include <pelopia/Dataset.h>

#include <test/catch.hpp>

#include <stdexcept>
#include <iostream>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "Dataset Construct from a non-existent file" ) 
{
    REQUIRE_THROWS_AS ( Dataset ds ( "./input/notthere" ), invalid_argument );
}

TEST_CASE ( "Dataset Construct from a bad JSON file" ) 
{
    REQUIRE_THROWS_AS ( Dataset ds ( "./input/invalid.json" ), logic_error );
}

TEST_CASE ( "Place" ) 
{
    Dataset ds ( "./input/one-term.json" );
    SECTION ( "InvalidId" )     { REQUIRE_THROWS_AS ( ds.Place(InvalidId), invalid_argument ); }
    SECTION ( "Id too high" )   { REQUIRE_THROWS_AS ( ds.Place(999999), invalid_argument ); }
    SECTION ( "Valid Id" )      { REQUIRE ( string ( "8 Warwickshire Road, Warwick, Bermuda" ) == ds.Place(2).Label() ); }
}

TEST_CASE ( "Search one term, all defaults, not found" ) 
{
    Dataset ds ( "./input/one-term.json" );
    const Response& resp = ds.Search("Notthereshire");
    REQUIRE ( 0 == resp.Count() );
}

TEST_CASE ( "Search one term, all defaults" ) 
{
    Dataset ds ( "./input/one-term.json" );
    const Response& resp = ds.Search("Warwickshire");
    
    SECTION ( "Found something" )   { REQUIRE ( 0 != resp.Count() ); }
    SECTION ( "Result count" )      { REQUIRE ( DefaultResults == resp.Count() ); }
    SECTION ( "Results sorted" )    
    {
        MatchQuality prevScore;
        for ( size_t i = 0; i < resp.Count(); ++i )
        {
            Id id;
            MatchQuality score = resp.Get(i, id, score);
            if ( i == 0 )
            {
                prevScore = score;
            }
            else
            {
                REQUIRE ( score <= prevScore );
            } 
        } 
    }
    SECTION ( "Id in Results do not repeat" )    
    {
        set<Id> ids;
        for ( size_t i = 0; i < resp.Count(); ++i )
        {
            Id id = resp.Get(i);
            REQUIRE ( ids.find(id) == ids.end() );
            ids.insert(id);
        } 
    }
    SECTION ( "Hits contain the term" )    
    {
        for ( size_t i = 0; i < resp.Count(); ++i )
        {
            Id id = resp.Get(i);
        } 
    }
}

