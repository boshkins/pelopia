#include <pelopia/Distance.h>

using namespace Mapzen :: Pelopia;
using namespace std;

static const double MilesToKm = 1.609344;

Distance::Distance()
: m_distanceKm(0.0)
{
}

Distance::Distance( Units p_units, double p_dist )
: m_distanceKm( p_units == Kilometers ? p_dist : ( p_dist * MilesToKm ) )
{
}

Distance::Distance( const Distance& p_that )
: m_distanceKm( p_that.m_distanceKm )
{
}

double
Distance::GetMiles() const
{
    return m_distanceKm / MilesToKm;
}

