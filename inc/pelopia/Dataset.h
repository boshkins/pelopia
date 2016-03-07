#ifndef Mapzen_Pelopia_Dataset_h
#define Mapzen_Pelopia_Dataset_h

/*
* Copyright notice: TBD
*/

/*
* API for Pelopia, an open source offline geocoding library
* Project repository: https://github.com/boshkins/pelopia
*/

#include <stdexcept>

#include <pelopia/Response.h>
#include <pelopia/Distance.h>
#include <pelopia/BoundingBox.h>
#include <pelopia/GeocodeJSON.h>

namespace Mapzen
{
	namespace Pelopia
	{
        class TextIndexReader;

		class Dataset
        {
		public:
			Dataset ( const char* filename ) throw ( std :: invalid_argument, std :: logic_error );
			~Dataset ();

            // For all methods returning const Response&, the reference is valid
            // until the next call to a Search/Autocomplete method
            const Response& Search (
                const char* text,
				Format format = { DefaultResults } );

			const Response& Search (
                const char* text,
				const LatLon& scope,
				const Distance& radius = Distance ( Distance::Miles, 0 ),
				Format format = { DefaultResults } );

			const Response& Search (
                const char* text,
				const BoundingBox& scope,
				Format format = { DefaultResults } );

			const Response& Autocomplete ( const char* text, const LatLon& scope );

			const Response& Autocomplete ( const char* text, const BoundingBox& scope );

			const Response& Reverse (
                const LatLon& point,
				const Distance& radius,
				Format format = { AllResults } );

            // The reference is valid until the next call to Place()
            // Use Feature::Clone() to make a caller-owned copy
			const GeocodeJSON :: Feature& Place ( Id ) const throw ( std :: invalid_argument );

            // An estimation of the Maximum Distance between features in the dataset
            Distance MaxDistance () const;

            // for library's internal use
            const TextIndexReader& GetIndex () const;
            size_t FeatureCount () const;

        private:
            class Impl;

            Impl* m_impl;
		};
	}
}

#endif
