#include "../FilteringGeoTextScorer.h"

#include "../NormalizerNaive.h"
#include "../LocationFilter.h"

#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "FilteringGeoTextScorer" )
{
    Dataset ds ( "input/one-term.json" );
    LatLon focus ( 30.21615, -81.598765 ); // Jacksonville, FL
    NormalizerNaive norm;
    Normalizer::Result normQuery;
    norm.Normalize ( "Warwickshire", normQuery );

    const double MaxKm = ds.MaxDistance().GetKilometers();
    SECTION ( "DeadCenter, filter set" )
    {
        LocationFilter filter( ds.FeatureCount() );
        filter.Set(9);
        FilteringGeoTextScorer s ( ds, filter, focus, norm, normQuery );
        REQUIRE ( 0 != s.Score ( 9 ) );
    }
    SECTION ( "DeadCenter, filter unset" )
    {
        LocationFilter filter( ds.FeatureCount() );
        FilteringGeoTextScorer s ( ds, filter, focus, norm, normQuery );
        REQUIRE ( 0 == s.Score ( 9 ) ); // filtered out
    }
}

