#include <pelopia/BoundingBox.h>

using namespace Mapzen :: Pelopia;
using namespace std;

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

