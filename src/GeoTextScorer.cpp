/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Feature Scorer with geo distance and text scoring
*/

#include "GeoTextScorer.h"

using namespace Mapzen::Pelopia;

GeoTextScorer::GeoTextScorer ( const Dataset & p_dataset,
                               const LatLon & p_center,
                               const Normalizer :: Result & query )
: Scorer ( p_dataset )
{
}

GeoTextScorer::~GeoTextScorer ()
{
}

MatchQuality
GeoTextScorer::Score ( Id ) const
{
    return 0.0;
}
