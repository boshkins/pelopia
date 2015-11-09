#ifndef Mapzen_Pelopia_Distance_h
#define Mapzen_Pelopia_Distance_h

/*
* Copyright notice: TBD
*/

/*
* API for Pelopia, an open source offline geocoding library
* Project repository: https://github.com/boshkins/pelopia
*/

namespace Mapzen
{
	namespace Pelopia
	{

		class Distance
		{
		public:
			typedef enum { Miles, Kilometers } Units;

			Distance(Units, double);

			double GetMiles() const;
			double GetKiloMetres() const;
		};

	}
}

#endif
