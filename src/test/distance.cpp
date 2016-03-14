#include <pelopia/Distance.h>

#include <test/catch.hpp>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "Distance Default Construct" )
{
    Distance d;
    REQUIRE ( 0.0 == d.GetMiles() );
    REQUIRE ( 0.0 == d.GetKilometers() );
}

TEST_CASE ( "Distance Construct Miles" )
{
    Distance d (Distance::Miles, 1);
    REQUIRE ( 1.0 == d.GetMiles() );
    REQUIRE ( 1.609344 == d.GetKilometers() );
}

TEST_CASE ( "Distance Construct Km" )
{
    Distance d (Distance::Kilometers, 2);
    REQUIRE ( Approx(1.24274) == d.GetMiles() );
    REQUIRE ( 2.0 == d.GetKilometers() );
}

TEST_CASE ( "Distance Copy Construct" )
{
    Distance d1 (Distance::Kilometers, 2);
    Distance d2 (d1);
    REQUIRE ( d1.GetMiles() == d2.GetMiles() );
    REQUIRE ( d1.GetKilometers() == d2.GetKilometers() );
}

