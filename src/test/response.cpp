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

