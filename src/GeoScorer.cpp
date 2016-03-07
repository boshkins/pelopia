/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Implementation of Feature Scorer with geo distance scoring
*/

#include "GeoScorer.h"

#include <cmath>

using namespace Mapzen::Pelopia;

const double Origin = 0.0;
const double Decay = 0.5;
const double ScaleFactor = 0.5;
const double OffsetKm = 0.0;

GeoScorer::GeoScorer ( const Dataset & p_dataset, const LatLon & p_center )
:   Scorer ( p_dataset ),
    m_center ( p_center ),
    m_lambda ( log ( Decay ) / ( GetDataset().MaxDistance().GetKilometers() * ScaleFactor ) )
{
}

GeoScorer::~GeoScorer ()
{
}

MatchQuality
GeoScorer::Score ( Id p_id ) const
{
    const GeocodeJSON::Geometry& coords = *GetDataset().Place( p_id ).GetGeometry();
    const LatLon latLon ( coords.Latitude(), coords.Longitude() );
    const double fromCenterKm = m_center.DistanceTo( latLon ).GetKilometers();
    if ( fromCenterKm > OffsetKm )
    {
        return exp ( m_lambda * ( fromCenterKm - OffsetKm ) );
    }
    return 1.0;
}
