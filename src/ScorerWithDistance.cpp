/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Implementation of Feature Scorer with text and distance scoring 
*/

#include "ScorerWithDistance.h"

#include <cmath>

using namespace Mapzen::Pelopia;

const double Origin = 0.0;
const double Decay = 0.5;
const double ScaleFactor = 0.5;
const double Offset = 0.0;

ScorerWithDistance::ScorerWithDistance ( const Dataset & p_dataset )
:   m_dataset ( p_dataset ), 
    m_coordsSet ( false ),
    m_lambda ( log ( Decay ) / ( m_dataset.MaxDistance().GetKilometers() * ScaleFactor ) ) 
{

}

ScorerWithDistance::~ScorerWithDistance ()
{
}

MatchQuality
ScorerWithDistance::GeoScore ( const Distance& p_fromCenter ) const
{
    if ( p_fromCenter.GetKilometers() > Offset )
    {
        return exp ( m_lambda * p_fromCenter.GetKilometers() - Offset );
    }
    return 1.0;
}

MatchQuality
ScorerWithDistance::TextScore () const
{
    return 1.0;
}

MatchQuality 
ScorerWithDistance::Score ( const GeocodeJSON::Feature & p_feature ) const
{
    MatchQuality ret = TextScore();
    if ( m_coordsSet )
    {
        LatLon latLon ( p_feature.GetGeometry()->Latitude(), p_feature.GetGeometry()->Longitude() );
        ret *= GeoScore ( m_coords.DistanceTo ( latLon ) );
    }
    return ret;
}
