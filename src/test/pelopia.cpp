#include <pelopia/Pelopia.h>

#include <test/catch.hpp>

using namespace Mapzen :: Pelopia;

TEST_CASE ( "LatLon ctor, getters, equality" )
{
    LatLon v ( 1.2, 3.4 );
    
    SECTION ( "getters")
    {
        REQUIRE ( 1.2 == v . Latitude () );
        REQUIRE ( 3.4 == v . Longitude () );
    }

    SECTION ( "equality")
    {
        REQUIRE ( LatLon ( 1.2, 3.4 ) == v );
        REQUIRE ( LatLon ( 2.0, 1.0 ) != v );
    }
}

TEST_CASE ( "LatLon ctor coordinates out of range" )
{
    LatLon v ( 0, 360 ); // ok
    REQUIRE_NOTHROW ( LatLon ( -90.0, 0 ) );
    REQUIRE_THROWS ( LatLon ( -90.01, 0 ) );
    REQUIRE_NOTHROW ( LatLon ( 90.0, 0 ) );
    REQUIRE_THROWS ( LatLon ( 90.01, 0 ) );
    REQUIRE_NOTHROW ( LatLon ( 0, -180.0 ) );
    REQUIRE_THROWS ( LatLon ( 0, -180.01 ) );
    REQUIRE_NOTHROW ( LatLon ( 0, 540.0 ) );
    REQUIRE_THROWS ( LatLon ( 0, 540.01 ) );
}

TEST_CASE ( "LatLon default ctor" )
{
    REQUIRE ( LatLon ( 0, 0 ) == LatLon() );
}

TEST_CASE ( "LatLon setters" )
{
    LatLon v ( 1.2, 3.4 );
    
    v . SetLatitude ( 4.5 );
    REQUIRE ( 4.5 == v . Latitude () );
    
    v . SetLongitude ( 6.7 );
    REQUIRE ( 6.7 == v . Longitude () );
}

TEST_CASE ( "LatLon setters coords out of range" )
{
    LatLon v ( 1.2, 3.4 );
    
    REQUIRE_NOTHROW ( v . SetLatitude ( -90.0 ) );
    REQUIRE_THROWS ( v . SetLatitude ( -90.01 ) );
    REQUIRE_NOTHROW ( v . SetLatitude ( 90.0 ) );
    REQUIRE_THROWS ( v . SetLatitude ( 90.01 ) );
    REQUIRE_NOTHROW ( v . SetLongitude ( -180.0 ) );
    REQUIRE_THROWS ( v . SetLongitude ( -180.01 ) );
    REQUIRE_NOTHROW ( v . SetLongitude ( 540.0 ) );
    REQUIRE_THROWS ( v . SetLongitude ( 540.01 ) );
}

TEST_CASE ( "LatLon DistanceTo self" )
{
    LatLon v ( 1.2, 3.4 );
    REQUIRE ( 0 == v.DistanceTo ( v ).GetKilometers() );
}

TEST_CASE ( "LatLon DistanceTo Manchester-Liverpool" )
{
    LatLon man ( 53.5, -2.2167 );
    LatLon liv ( 53.4167, -3.0 );
    REQUIRE ( 52.68 == Approx ( man.DistanceTo ( liv ).GetKilometers() ).epsilon( 0.001 ) );
}


