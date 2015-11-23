#ifndef Mapzen_Pelopia_BoundingBox_h
#define Mapzen_Pelopia_BoundingBox_h

/*
* Copyright notice: TBD
*/

/*
* API for Pelopia, an open source offline geocoding library
* Project repository: https://github.com/boshkins/pelopia
*/

#include <pelopia/Pelopia.h>

namespace Mapzen
{
	namespace Pelopia
	{
		class BoundingBox {
		public:
			// all constructors will recalculate longitudes to 0 .. 360 if the 180 
			//  meridian lies within the box
			BoundingBox (); // all 0s
			BoundingBox ( LatLon p_topLeft, LatLon p_bottomRight );
			BoundingBox (
                Coordinate lonLeft,
				Coordinate latBottom,
				Coordinate lonRight,
				Coordinate latTop );

			Coordinate Left() const     { return m_topLeft . Longitude (); }
			Coordinate Bottom() const   { return m_bottomRight . Latitude (); }
			Coordinate Right() const    { return m_bottomRight . Longitude (); }
			Coordinate Top() const      { return m_topLeft . Latitude (); }

			const LatLon& TopLeft() const { return m_topLeft; }
			const LatLon& BottomRight() const { return m_bottomRight; }
            
            bool operator == ( const BoundingBox& p_that ) const 
            {
                return m_topLeft == p_that . m_topLeft && m_bottomRight == p_that . m_bottomRight;
            }
            bool operator != ( const BoundingBox& p_that ) const 
            {
                return ! ( *this == p_that );
            }
            
			void SetLeft    ( Coordinate );
			void SetBottom  ( Coordinate );
			void SetRight   ( Coordinate );
			void SetTop     ( Coordinate );

			void SetTopLeft ( const LatLon& ) const;
			void SetBottomRight ( const LatLon& ) const;
            
		private:
			LatLon m_topLeft;
			LatLon m_bottomRight;
		};
	}
}

#endif
