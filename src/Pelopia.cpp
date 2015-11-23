#include <pelopia/Pelopia.h>

#include <sstream>

using namespace Mapzen :: Pelopia;
using namespace std;

static const Coordinate MinLatitude = -90.0;
static const Coordinate MaxLatitude = 90.0;

static const Coordinate MinLongitude = -180.0;
static const Coordinate MaxLongitude = 360.0;

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
LatLon :: SetLongitude ( Coordinate p_lon )
{
    CheckLongitude ( p_lon );
    m_lon = p_lon;
}
