#include "../ResponseInternal.h"

#include <test/catch.hpp>

#include <stdexcept>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "Construct" ) 
{
    Response resp;
    REQUIRE ( 0 == resp.Count() );
}

TEST_CASE ( "Match" ) 
{
    ResponseInternal resp ( 2 );
    resp.AppendMatch ( 1, 0.5 );
    resp.AppendMatch ( 2, 1.5 );
    
    Id id = InvalidId;
    MatchQuality score = 0;
    
    SECTION ( "Count" )
    {
        REQUIRE ( 2 == resp.Count() );
    }
    
    SECTION ("Get_Full")
    {
        REQUIRE ( resp.Get ( 0, id, score ) );
        REQUIRE ( 1 == id );
        REQUIRE ( 0.5 == score );
        
        REQUIRE ( resp.Get ( 1, id, score ) );
        REQUIRE ( 2 == id );
        REQUIRE ( 1.5 == score );
    }
    
    SECTION ("Get_Full_out_of_bounds")
    {
        REQUIRE ( ! resp.Get ( 2, id, score ) );
    }
    
    SECTION ("Get_String")
    {
        REQUIRE ( 1 == resp.Get ( 0 ) );
        REQUIRE ( 2 == resp.Get ( 1 ) );
    }
    
    SECTION ("Get_String_out_of_bounds")
    {
        REQUIRE ( InvalidId == resp.Get ( 2 ) );
    }
    
}

TEST_CASE ( "Clear" ) 
{
    ResponseInternal resp;
    resp.AppendMatch ( 1, 0.5 );
    resp.AppendMatch ( 2, 1.5 );
    resp.AppendMatch ( 3, 0.6 );
    resp.AppendMatch ( 4, 1.2 );
    resp.Clear();
    REQUIRE ( 0 == resp.Count() );
}

TEST_CASE ( "Sort" ) 
{
    ResponseInternal resp;
    resp.AppendMatch ( 1, 0.5 );
    resp.AppendMatch ( 2, 1.5 );
    resp.AppendMatch ( 3, 0.6 );
    resp.AppendMatch ( 4, 1.2 );
    
    resp.Sort(); // descending score

    REQUIRE ( 2 == resp.Get ( 0 ) );
    REQUIRE ( 4 == resp.Get ( 1 ) );
    REQUIRE ( 3 == resp.Get ( 2 ) );
    REQUIRE ( 1 == resp.Get ( 3 ) );
}

TEST_CASE ( "Truncate, no change" ) 
{
    ResponseInternal resp;
    resp.AppendMatch ( 1, 0.5 );
    resp.AppendMatch ( 2, 1.5 );
    resp.AppendMatch ( 3, 0.6 );
    resp.AppendMatch ( 4, 1.2 );
    resp.Truncate(4);
    REQUIRE ( 4 == resp.Count() );
}

TEST_CASE ( "Truncate, changed" ) 
{
    ResponseInternal resp;
    resp.AppendMatch ( 1, 0.5 );
    resp.AppendMatch ( 2, 1.5 );
    resp.AppendMatch ( 3, 0.6 );
    resp.AppendMatch ( 4, 1.2 );
    resp.Truncate(2);
    REQUIRE ( 2 == resp.Count() );
    REQUIRE ( 1 == resp.Get ( 0 ) );
    REQUIRE ( 2 == resp.Get ( 1 ) );
}

