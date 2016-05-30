/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Feature Scorer with geo distance and text scoring, and externally provided filter.
*/

#include "FilteringGeoTextScorer.h"

using namespace Mapzen::Pelopia;

FilteringGeoTextScorer::FilteringGeoTextScorer ( const Dataset & p_dataset,
                                                 const LocationFilter&,
                                                 const LatLon & p_center,
                                                 const Normalizer :: Result & query )
: Scorer ( p_dataset )
{
}

FilteringGeoTextScorer::~FilteringGeoTextScorer ()
{
}

MatchQuality
FilteringGeoTextScorer::Score ( Id ) const
{
    return 0.0;
}
