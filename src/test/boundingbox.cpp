#include <pelopia/BoundingBox.h>

#include <test/catch.hpp>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "BoundingBox getters" )
{
    BoundingBox b ( 1.2, 3.4, 5.6, 7.8 );
    
    SECTION ( "Coordinate" )
    {
        REQUIRE ( 1.2 == b . Left () );
        REQUIRE ( 3.4 == b . Bottom () );
        REQUIRE ( 5.6 == b . Right () );
        REQUIRE ( 7.8 == b . Top () );
    }
    
    SECTION ( "LatLon" )
    {
        REQUIRE ( LatLon ( 7.8, 1.2 ) == b . TopLeft () );
        REQUIRE ( LatLon ( 3.4, 5.6 ) == b . BottomRight () );
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

TEST_CASE ( "BoundingBox setters" )
{
    BoundingBox b;
    
    b . SetLeft ( 1.2 );
    b . SetBottom ( 3.4 );
    b . SetRight ( 5.6 );
    b . SetTop ( 7.8 );
    REQUIRE ( BoundingBox ( 1.2, 3.4, 5.6, 7.8 ) == b );
}

//TEST_CASE ( "BoundingBox setters recalculate" )

