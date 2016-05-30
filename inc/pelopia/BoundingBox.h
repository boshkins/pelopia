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
            // All 4-Coordinate calls follow the order
            // Top - Left - Bottom - Right
            //
            // All 2-LatLon calls follow the order
            // TopLeft - BottomRight

			// all constructors will recalculate longitudes to 0 .. 540 if the 180
			//  meridian lies within the box
			BoundingBox (); // all 0s
			BoundingBox ( LatLon p_topLeft, LatLon p_bottomRight )  throw ( std :: logic_error );
			BoundingBox (
				Coordinate latTop,
                Coordinate lonLeft,
				Coordinate latBottom,
				Coordinate lonRight )  throw ( std :: logic_error );

			// A Bounding Box enclosing a circle with the given center and radius,
			// using equirectangular approximation of the Earth's surface'
			BoundingBox ( LatLon p_center, const Distance& p_dist )  throw ( std :: logic_error );

			Coordinate Top() const      { return m_topLeft . Latitude (); }
			Coordinate Left() const     { return m_topLeft . Longitude (); }
			Coordinate Bottom() const   { return m_bottomRight . Latitude (); }
			Coordinate Right() const    { return m_bottomRight . Longitude (); }

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

			// the setters take care of translation of longitude around the 180 meridian, if necessary
			void SetCoordinates ( const LatLon& p_topLeft,  const LatLon& p_bottomRight )  throw ( std :: logic_error );
			void SetCoordinates (
				Coordinate latTop,
				Coordinate lonLeft,
				Coordinate latBottom,
				Coordinate lonRight )  throw ( std :: logic_error );

		private:
			LatLon m_topLeft;
			LatLon m_bottomRight;
		};
	}
}

#endif