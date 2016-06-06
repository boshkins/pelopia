#include "../GeoTextScorer.h"

#include "../NormalizerNaive.h"


#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "GeoTextScorer Score" )
{
    Dataset ds ( "input/one-term.json" );
    LatLon focus ( 30.21615, -81.598765 ); // Jacksonville, FL
    NormalizerNaive norm;
    Normalizer::Result normQuery;
    norm.Normalize ( "Warwickshire", normQuery );
    GeoTextScorer s ( ds, focus, norm, normQuery );
    TextScorer ts ( ds, norm, normQuery );

    const double MaxKm = ds.MaxDistance().GetKilometers();
    SECTION ( "DeadCenter" )
    {
        REQUIRE ( ts.Score ( 9 ) * 1.0 == s.Score ( 9 ) ); // Jacksonville, FL
    }
    SECTION ( "Nearby" )
    {
        REQUIRE ( ts.Score ( 10 ) * 0.9998 == Approx ( s.Score ( 10 ) ) .epsilon( 0.0001 ) ); // Jacksonville, FL
    }
    SECTION ( "FarAway" )
    {
        REQUIRE ( ts.Score ( 1 ) * 0.3951 == Approx ( s.Score ( 1 ) ) .epsilon( 0.0001 ) ); // Buckingham, PA
    }
    SECTION ( "FarFarAway" )
    {
        REQUIRE ( ts.Score ( 2 ) * 0.3068 == Approx ( s.Score ( 2 ) ) .epsilon( 0.0001 ) ); // Warwick, Bermuda
    }

}

