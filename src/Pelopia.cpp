#include <pelopia/Pelopia.h>

#include <sstream>
#include <cmath>

using namespace Mapzen :: Pelopia;
using namespace std;

static const Coordinate MinLatitude = -90.0;
static const Coordinate MaxLatitude = 90.0;

static const Coordinate MinLongitude = -180.0;
static const Coordinate MaxLongitude = 180.0 + 360.0;

static
void
CheckLatitude ( Coordinate p_lat ) throw ( std :: logic_error )
{
    if ( p_lat < MinLatitude || p_lat > MaxLatitude )
    {
        ostringstream s;
        s << "Latitude value " << p_lat << " is outside of allowed range " << (int)MinLatitude << ".." << (int)MaxLatitude;
        throw std :: logic_error ( s . str() ); 
    }
}

static
void
CheckLongitude ( Coordinate p_lon ) throw ( std :: logic_error )
{
    if ( p_lon < MinLongitude || p_lon > MaxLongitude )
    {
        ostringstream s;
        s << "Latitude value " << p_lon << " is outside of allowed range " << (int)MinLongitude << ".." << (int)MaxLongitude;
        throw std :: logic_error ( s . str() ); 
    }
}

LatLon :: LatLon ()
: m_lat ( 0 ), m_lon ( 0 )
{
}

LatLon :: LatLon ( Coordinate p_lat, Coordinate p_lon ) throw ( std :: logic_error )
: m_lat ( p_lat ), m_lon ( p_lon )
{
    CheckLatitude ( m_lat );
    CheckLongitude ( m_lon );
}

void 
LatLon :: SetLatitude  ( Coordinate p_lat )  throw ( std :: logic_error )
{
    CheckLatitude ( p_lat );
    m_lat = p_lat;
}

void 
LatLon :: SetLongitude ( Coordinate p_lon )  throw ( std :: logic_error )
{
    CheckLongitude ( p_lon );
    m_lon = p_lon;
}

static
double
DegreesToRadians ( double p_degrees )
{
    return p_degrees * 0.0174533;
}

Distance 
LatLon :: DistanceTo ( const LatLon& p_that ) const
{   // use equirectangular approximation ( see http://www.movable-type.co.uk/scripts/latlong.html )
    double lat1 = DegreesToRadians ( m_lat );
    double lat2 = DegreesToRadians ( p_that.m_lat );
    double lon1 = DegreesToRadians ( m_lon );
    double lon2 = DegreesToRadians ( p_that.m_lon );
    auto x = ( lon2 - lon1 ) * cos( ( lat1 + lat2 )  / 2.0 );
    auto y = lat2 - lat1;
    const double EarthRadius = 6371.0;
    return Distance ( Distance::Kilometers, sqrt ( x * x + y * y ) * EarthRadius ); 
}