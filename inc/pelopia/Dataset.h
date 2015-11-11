#ifndef Mapzen_Pelopia_Dataset_h
#define Mapzen_Pelopia_Dataset_h

/*
* Copyright notice: TBD
*/

/*
* API for Pelopia, an open source offline geocoding library
* Project repository: https://github.com/boshkins/pelopia
*/

#include <pelopia/Response.h>
#include <pelopia/Distance.h>
#include <pelopia/BoundingBox.h>
#include <pelopia/GeocodeJSON.h>

namespace Mapzen
{
	namespace Pelopia
	{

		class Dataset {

		public:
			Dataset(const char* filename);

			Response Search(const char* text, 
				Format format = { DefaultResults });

			Response Search(const char* text,
				const LatLon& scope,
				const Distance& radius = Distance ( Distance::Miles, 0 ),
				Format format = { DefaultResults });

			Response Search(const char* text,
				const BoundingBox& scope,
				Format format = { DefaultResults });

			Response Autocomplete(const char* text, const LatLon& scope);

			Response Autocomplete(const char* text, const BoundingBox& scope);

			Response Reverse(const LatLon& point,
				const Distance& radius,
				Format format = { AllResults });

			GeocodeJSON :: Feature Place(Id) const;
		};

	}
}

#endif
