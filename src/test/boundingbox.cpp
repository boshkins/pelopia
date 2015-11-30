#include <pelopia/BoundingBox.h>

#include <test/catch.hpp>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "BoundingBox getters" )
{
    BoundingBox b ( 1.2, 3.4, 5.6, 7.8 );
    
    SECTION ( "Coordinate" )
    {
        REQUIRE ( 1.2 == b . Top () );
        REQUIRE ( 3.4 == b . Left () );
        REQUIRE ( 5.6 == b . Bottom () );
        REQUIRE ( 7.8 == b . Right () );
    }
    
    SECTION ( "LatLon" )
    {
        REQUIRE ( LatLon ( 1.2, 3.4 ) == b . TopLeft () );
        REQUIRE ( LatLon ( 5.6, 7.8 ) == b . BottomRight () );
    }
    
    SECTION ( "equality" )
    {
        BoundingBox b1 ( 1.2, 3.4, 5.6, 7.8 );
        REQUIRE ( b == b1 );
        REQUIRE ( ! ( b != b1 ) );
    }
/*    
    SECTION ( "assignment" )
    {
        BoundingBox b1 ( 3.4, 5.6, 7.8, 9.1 );
        b1 = b;
        REQUIRE ( b1 == b1 );
    }
*/    
}

TEST_CASE ( "BoundingBox default ctor" )
{
    BoundingBox b;
    REQUIRE ( BoundingBox ( 0, 0, 0, 0 ) == b );
}

TEST_CASE ( "BoundingBox setter using Coordinates" )
{
    BoundingBox b;
    b . SetCoordinates ( 1.2, 3.4, 5.6, 7.8 );
    REQUIRE ( BoundingBox ( 1.2, 3.4, 5.6, 7.8 ) == b );
}

TEST_CASE ( "BoundingBox setter using LatLons" )
{
    BoundingBox b;
    b . SetCoordinates ( LatLon ( 1.2, 3.4 ), LatLon ( 5.6, 7.8 ) );
        REQUIRE ( 1.2 == b . Top () );
        REQUIRE ( 3.4 == b . Left () );
        REQUIRE ( 5.6 == b . Bottom () );
        REQUIRE ( 7.8 == b . Right () );
            REQUIRE ( BoundingBox ( 1.2, 3.4, 5.6, 7.8 ) == b );
}

TEST_CASE ( "BoundingBox setter using Coordinates, recalculate right latitudes" )
{
    BoundingBox b;
    b . SetCoordinates ( 1, 160.0, 2, -170 ); // left > right
    REQUIRE ( BoundingBox ( 1, 160, 2, 360 - 170 ) == b ); // new right = 360 + old right
}
TEST_CASE ( "BoundingBox setter using LatLon, recalculate right latitudes" )
{
    BoundingBox b;
    b . SetCoordinates ( LatLon ( 1, 160 ), LatLon ( 2, -170 ) ); // left > right
    REQUIRE ( BoundingBox ( 1, 160, 2, 360 - 170 ) == b ); // new right = 360 + old right
}


