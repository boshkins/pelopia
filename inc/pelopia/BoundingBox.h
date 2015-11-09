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
			// all constructors will recalculate right longitude to above 180 degrees if the 180 
			//  meridian lies within the box
			BoundingBox(LatLon p_topLeft, LatLon p_bottomRight);
			BoundingBox(Coordinate latLeft,
				Coordinate latRight,
				Coordinate lonBottom,
				Coordinate lonTop);

			const Coordinate Left() const;
			const Coordinate Right() const;
			const Coordinate Top() const;
			const Coordinate Bottom() const;

			const LatLon& TopLeft() const;
			const LatLon& BottomRight() const;

		private:
			LatLon topLeft;
			LatLon bottomRight;
		};


	}
}

#endif
