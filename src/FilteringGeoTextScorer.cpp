/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Feature Scorer with geo distance and text scoring, and externally provided filter.
*/

#include "FilteringGeoTextScorer.h"

#include "LocationFilter.h"

using namespace Mapzen::Pelopia;

FilteringGeoTextScorer::FilteringGeoTextScorer ( const Dataset &                p_dataset,
                                                 const LocationFilter&          p_filter,
                                                 const LatLon &                 p_center,
                                                 const Normalizer &             p_normalizer,
                                                 const Normalizer :: Result &   p_query )
:   GeoTextScorer ( p_dataset, p_center, p_normalizer, p_query ),
    m_filter ( p_filter )
{
}

FilteringGeoTextScorer::~FilteringGeoTextScorer ()
{
}

MatchQuality
FilteringGeoTextScorer::Score ( Id p_id ) const
{
    return m_filter.IsSet( p_id ) ? GeoTextScorer::Score( p_id ) : 0;
}
