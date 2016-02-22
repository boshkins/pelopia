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

TEST_CASE ( "Dataset Search one term, all defaults, not found" ) 
{
    Dataset ds ( "./input/one-term.json" );
    const Response& resp = ds.Search("Notthereshire");
    REQUIRE ( 0 == resp.Count() );
}

TEST_CASE ( "Dataset Search one term, all defaults" ) 
{
    Dataset ds ( "./input/one-term.json" );
    const char Term[] = "Warwickshire";
    const Response& resp = ds.Search( Term );
    
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
            REQUIRE ( string( ds.Place( id ).Label () ).find( Term ) != string::npos );
        } 
    }
}

TEST_CASE ( "Dataset Search one term, focus LatLon, unbound" ) 
{
    Dataset ds ( "./input/one-term.json" );
    LatLon focus ( -81.598765, 30.21615 ); 
    Distance distance ( Distance :: Miles, 0 );
    const Response& resp = ds.Search ( "Warwickshire", focus, distance );
    REQUIRE ( DefaultResults == resp.Count() ); 
    
    // TODO: check scores, sort order
    
    REQUIRE ( false ); 
}

TEST_CASE ( "Dataset Search one term, focus LatLon, bound" ) 
{
    Dataset ds ( "./input/one-term.json" );
    LatLon focus ( -81.598765, 30.21615 ); 
    Distance distance ( Distance :: Miles, 10.0 );
    const Response& resp = ds.Search ( "Warwickshire", focus, distance );
    REQUIRE ( 3 == resp.Count() ); 
    // TODO: check scores, sort order
}

TEST_CASE ( "Dataset MaxDistance" ) 
{
    Dataset ds ( "./input/one-term.json" );
    REQUIRE ( LatLon ( 30.214098, -81.598765 ).DistanceTo ( LatLon (40.292543, -64.810274) ).GetKilometers() == Approx ( ds.MaxDistance().GetKilometers() ).epsilon ( 0.01 ) );
}
