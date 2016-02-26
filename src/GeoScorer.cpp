/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Implementation of Feature Scorer with text and distance scoring 
*/

#include "GeoScorer.h"

#include <cmath>

using namespace Mapzen::Pelopia;

const double Origin = 0.0;
const double Decay = 0.5;
const double ScaleFactor = 0.5;
const double OffsetKm = 0.0;

GeoScorer::GeoScorer ( const Dataset & p_dataset, const LatLon & p_center )
:   m_dataset ( p_dataset ), 
    m_center ( p_center ),
    m_lambda ( log ( Decay ) / ( m_dataset.MaxDistance().GetKilometers() * ScaleFactor ) ) 
{
}

GeoScorer::~GeoScorer ()
{
}


#include <iostream>

MatchQuality 
GeoScorer::Score ( const GeocodeJSON::Feature & p_feature ) const
{
    const LatLon latLon ( p_feature.GetGeometry()->Latitude(), p_feature.GetGeometry()->Longitude() );
std::cout << "center=" << m_center.Latitude() << ", " << m_center.Longitude()  << std::endl; 
std::cout << "latLon=" << latLon.Latitude() << ", " << latLon.Longitude()  << std::endl; 
    
    const double fromCenterKm = m_center.DistanceTo( latLon ).GetKilometers();
std::cout << "fromCenterKm=" << fromCenterKm << std::endl; 
    if ( fromCenterKm > OffsetKm )
    {
        return exp ( m_lambda * ( fromCenterKm - OffsetKm ) );
    }
    return 1.0;
}
