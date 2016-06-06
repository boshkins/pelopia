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

GeoTextScorer::GeoTextScorer ( const Dataset &              p_dataset,
                               const LatLon &               p_center,
                               const Normalizer &           p_normalizer,
                               const Normalizer :: Result & p_query )
:   Scorer ( p_dataset ),
    m_geo ( p_dataset, p_center ),
    m_text ( p_dataset, p_normalizer, p_query)
{
}

GeoTextScorer::~GeoTextScorer ()
{
}

MatchQuality
GeoTextScorer::Score ( Id p_id ) const
{
    return m_geo.Score( p_id ) * m_text.Score( p_id );
}
