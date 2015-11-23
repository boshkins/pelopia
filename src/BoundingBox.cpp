#include "Data.h"

using namespace Mapzen :: Pelopia;

Data::~Data()
{
}

#include <pelopia/BoundingBox.h>

using namespace Mapzen :: Pelopia;

BoundingBox :: BoundingBox()
:   m_topLeft ( 0, 0 ), m_bottomRight ( 0, 0 )
{
}

BoundingBox :: BoundingBox(
    Coordinate lonLeft,
    Coordinate latBottom,
    Coordinate lonRight,
    Coordinate latTop )
:   m_topLeft ( latTop, lonLeft ), m_bottomRight ( latBottom, lonRight )
{
}

void 
BoundingBox :: SetLeft ( Coordinate )
{
}

void 
BoundingBox :: SetBottom ( Coordinate )
{
}

void 
BoundingBox :: SetRight ( Coordinate )
{
}

void 
BoundingBox :: SetTop ( Coordinate )
{
}

