#include "../GeoScorer.h"

#include "../NormalizerNaive.h"


#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "GeoScorer Construct" ) 
{
    Dataset ds ( "input/one-term.json" );
    LatLon focus ( 30.21615, -81.598765 );
    GeoScorer s ( ds, focus );
    
    REQUIRE ( focus == s.GetCenter () );
}

TEST_CASE ( "GeoScorer GeoScore" ) 
{
    Dataset ds ( "input/one-term.json" );
    LatLon focus ( 30.21615, -81.598765 ); // Jacksonville, FL
    GeoScorer s ( ds, focus );
    
    const double MaxKm = ds.MaxDistance().GetKilometers();
    SECTION ( "DeadCenter" )
    {
        REQUIRE ( 1.0 == s.Score ( ds.Place ( 9 ) ) );
    } 
    SECTION ( "Nearby" )
    {
        REQUIRE ( 0.9998 == Approx ( s.Score ( ds.Place ( 10 ) ) ) .epsilon( 0.0001 ) ); // Jacksonville, FL
    } 
    SECTION ( "FarAway" )
    {
        REQUIRE ( 0.3951 == Approx ( s.Score ( ds.Place ( 1 ) ) ) .epsilon( 0.0001 ) ); // Buckingham, PA
    } 
    SECTION ( "FarFarAway" )
    {
        REQUIRE ( 0.3068 == Approx ( s.Score ( ds.Place ( 2 ) ) ) .epsilon( 0.0001 ) ); // Warwick, Bermuda
    } 
}

