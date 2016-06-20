#include "../LocationFilter.h"

#include <test/catch.hpp>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "LocationFilter" )
{
    SECTION( "Construction, MaxId, IsSet" )
    {
        LocationFilter f (10);
        REQUIRE ( 10 == f.MaxId() );
        for ( Id i = 0 ; i <= f.MaxId() - 1; ++i )
        {
            REQUIRE ( ! f.IsSet( i + 1 ) );
        }
    }

    SECTION( "Bad IsSet, InvalidId" )
    {
        LocationFilter f ( 4 );
        REQUIRE_THROWS ( f.IsSet( InvalidId ) );
    }
    SECTION( "Bad IsSet, Id too high" )
    {
        LocationFilter f ( 4 );
        REQUIRE_THROWS ( f.IsSet( 5 ) );
    }

    SECTION( "Bad Set, InvalidId" )
    {
        LocationFilter f ( 4 );
        REQUIRE_THROWS ( f.Set( InvalidId ) );
    }
    SECTION( "Bad Set, Id too high" )
    {
        LocationFilter f ( 4 );
        REQUIRE_THROWS ( f.Set( 5 ) );
    }
    SECTION( "Set" )
    {
        LocationFilter f (4);
        f.Set(1);
        f.Set(3);
        REQUIRE (   f.IsSet( 1 ) );
        REQUIRE ( ! f.IsSet( 2 ) );
        REQUIRE (   f.IsSet( 3 ) );
        REQUIRE ( ! f.IsSet( 4 ) );
    }

    SECTION( "Bad Unset, InvalidId" )
    {
        LocationFilter f ( 4 );
        REQUIRE_THROWS ( f.Unset( InvalidId ) );
    }
    SECTION( "Bad Unset, Id too high" )
    {
        LocationFilter f ( 4 );
        REQUIRE_THROWS ( f.Unset( 5 ) );
    }
    SECTION( "Unset" )
    {
        LocationFilter f (4);
        f.Set(1);
        f.Set(2);
        f.Set(3);
        f.Set(4);

        f.Unset(1);
        f.Unset(3);

        REQUIRE ( ! f.IsSet( 1 ) );
        REQUIRE (   f.IsSet( 2 ) );
        REQUIRE ( ! f.IsSet( 3 ) );
        REQUIRE (   f.IsSet( 4 ) );
    }

    SECTION( "And sizes differ" )
    {
        LocationFilter f1 ( 4 );
        LocationFilter f2 ( 3 );
        REQUIRE_THROWS ( f1.And( f2 ) );
    }
    SECTION( "And" )
    {
        LocationFilter f1 (4);
        f1.Set(1);
        f1.Set(2);
        f1.Set(3);
        LocationFilter f2 (4);
        f2.Set(1);
        f2.Set(3);
        f2.Set(4);

        f1.And(f2);

        REQUIRE (   f1.IsSet( 1 ) );
        REQUIRE ( ! f1.IsSet( 2 ) );
        REQUIRE (   f1.IsSet( 3 ) );
        REQUIRE ( ! f1.IsSet( 4 ) );
    }
}

