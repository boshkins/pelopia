#include <pelopia/BoundingBox.h>

#include <cmath>

using namespace Mapzen :: Pelopia;
using namespace std;

const double PI = 3.141592653589793;

static double MIN_LAT = DegreesToRadians ( -90);  // -PI/2
static double MAX_LAT = DegreesToRadians (  90);   //  PI/2
static double MIN_LON = DegreesToRadians (-180); // -PI
static double MAX_LON = DegreesToRadians ( 180);  //  PI

BoundingBox :: BoundingBox()
:   m_topLeft ( 0, 0 ), m_bottomRight ( 0, 0 )
{
}

BoundingBox :: BoundingBox(
    Coordinate p_latTop,
    Coordinate p_lonLeft,
    Coordinate p_latBottom,
    Coordinate p_lonRight )  throw ( logic_error )
{
    SetCoordinates ( p_latTop, p_lonLeft, p_latBottom, p_lonRight );
}

// A Bounding Box enclosing a circle with the given center and radius,
// using equirectangular approximation of the Earth's surface'
BoundingBox :: BoundingBox ( LatLon p_center, const Distance& p_dist )  throw ( std :: logic_error )
{   // based on https://github.com/anthonyvscode/LonelySharp/blob/master/LonelySharp/GeoLocation.cs
    double radLat = DegreesToRadians ( p_center.Latitude() );
    double radLon = DegreesToRadians ( p_center.Longitude() );

    // angular distance in radians on a great circle
    double radDist = p_dist.GetKilometers() / EarthRadius.GetKilometers();

    double minLat = radLat - radDist;
    double maxLat = radLat + radDist;

    double minLon, maxLon;
    if ( minLat > MIN_LAT && maxLat < MAX_LAT )
    {
        double deltaLon = asin ( sin ( radDist ) / cos ( radLat ) );
        minLon = radLon - deltaLon;
        if ( minLon < MIN_LON )
        {
            minLon += 2 * PI;
        }
        maxLon = radLon + deltaLon;
        if ( maxLon > MAX_LON )
        {
            maxLon -= 2 * PI;
        }
    }
    else
    {
        // a pole is within the distance
        minLat = max ( minLat, MIN_LAT );
        maxLat = min ( maxLat, MAX_LAT );
        minLon = MIN_LON;
        maxLon = MAX_LON;
    }

    m_topLeft.SetLatitude       ( RadiansToDegrees ( maxLat ) );
    m_topLeft.SetLongitude      ( RadiansToDegrees ( minLon ) );
    m_bottomRight.SetLatitude   ( RadiansToDegrees ( minLat ) );
    m_bottomRight.SetLongitude  ( RadiansToDegrees ( maxLon ) );
}

void
BoundingBox :: SetCoordinates  ( const LatLon& p_topLeft,  const LatLon& p_bottomRight )  throw ( logic_error )
{
    SetCoordinates ( p_topLeft . Latitude (), p_topLeft . Longitude (), p_bottomRight . Latitude (), p_bottomRight . Longitude () );
}

void
BoundingBox :: SetCoordinates (
    Coordinate p_latTop,
    Coordinate p_lonLeft,
    Coordinate p_latBottom,
    Coordinate p_lonRight )  throw ( logic_error )
{
    //TBD: catch additional errors in left/right

    // adjust right latitude to be greater than the left
    if ( p_lonLeft > p_lonRight )
    {
        p_lonRight += 360.0;
    }

    m_topLeft . SetLatitude ( p_latTop );
    m_topLeft . SetLongitude ( p_lonLeft );
    m_bottomRight . SetLatitude ( p_latBottom );
    m_bottomRight . SetLongitude ( p_lonRight );
}

