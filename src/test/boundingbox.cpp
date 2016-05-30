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

TEST_CASE ( "BoundingBox ctor around a circle" )
{
    BoundingBox b (LatLon ( 1, 2 ), Distance ( Distance :: Kilometers, 100 ) );
    REQUIRE ( 1.89932 == Approx ( b . Top () ).epsilon ( 0.00001 ) );
    REQUIRE ( 1.10054 == Approx ( b . Left () ).epsilon ( 0.00001 )  );
    REQUIRE ( 0.10067 == Approx ( b . Bottom () ).epsilon ( 0.00001 )  );
    REQUIRE ( 2.89945 == Approx ( b . Right () ).epsilon ( 0.00001 )  );
}
TEST_CASE ( "BoundingBox ctor around a circle, near North Pole" )
{
    BoundingBox b (LatLon ( 89, 2 ), Distance ( Distance :: Kilometers, 100 ) );
    REQUIRE ( 89.89932 == Approx ( b . Top () ).epsilon ( 0.00001 ) );
    REQUIRE ( -62.07464 == Approx ( b . Left () ).epsilon ( 0.00001 )  );
    REQUIRE ( 88.10067 == Approx ( b . Bottom () ).epsilon ( 0.00001 )  );
    REQUIRE ( 66.07464 == Approx ( b . Right () ).epsilon ( 0.00001 )  );
}
TEST_CASE ( "BoundingBox ctor around a circle, North Pole included" )
{
    BoundingBox b (LatLon ( 89, 2 ), Distance ( Distance :: Kilometers, 200 ) );
    REQUIRE ( 90 == b . Top () );
    REQUIRE ( -180 == b . Left () );
    REQUIRE ( 87.20135 == Approx ( b . Bottom () ).epsilon ( 0.00001 )  );
    REQUIRE ( 180 == b . Right () );
}
TEST_CASE ( "BoundingBox ctor around a circle, near South Pole" )
{
    BoundingBox b (LatLon ( -89, 2 ), Distance ( Distance :: Kilometers, 100 ) );
    REQUIRE ( -88.10067 == Approx ( b . Top () ).epsilon ( 0.00001 ) );
    REQUIRE ( -62.07464 == Approx ( b . Left () ).epsilon ( 0.00001 )  );
    REQUIRE ( -89.89932 == Approx ( b . Bottom () ).epsilon ( 0.00001 )  );
    REQUIRE ( 66.07464 == Approx ( b . Right () ).epsilon ( 0.00001 )  );
}
TEST_CASE ( "BoundingBox ctor around a circle, South Pole included" )
{
    BoundingBox b (LatLon ( -89, 2 ), Distance ( Distance :: Kilometers, 200 ) );
    REQUIRE ( -87.20135 == Approx ( b . Top () ).epsilon ( 0.00001 ) );
    REQUIRE ( -180 == b . Left () );
    REQUIRE ( -90 == b . Bottom () );
    REQUIRE ( 180 == b . Right () );
}
TEST_CASE ( "BoundingBox ctor around a circle, negative radius" )
{
    REQUIRE_THROWS ( BoundingBox (LatLon ( -89, 2 ), Distance ( Distance :: Kilometers, -200 ) ) );
}



