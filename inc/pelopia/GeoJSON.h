#ifndef Mapzen_Pelopia_GeoJSON_h
#define Mapzen_Pelopia_GeoJSON_h

/*
* Copyright notice: TBD
*/

/*
* Pelopia :: GeoJSON, declarations related to GeoJSON objects
*/

#include <pelopia/Pelopia.h>

#include <string>

namespace Mapzen
{
	namespace Pelopia
	{
		/**
		* GeoJSON declarations, based on http://geojson.org/geojson-spec.html](http://geojson.org/geojson-spec.html
		*/
		namespace GeoJSON {

			/**
			* GeoJSON Geometry. For now, only Point in Geographic Coordinate Reference System
			*/
			class Geometry
			{	//  
			public:
				Coordinate Latitude() const;
				Coordinate Longitude() const;
			};

			/**
			* GeoJSON Feature object corresponding to a single Location
			*/
			class Feature
			{
			public:
				const Geometry* GetGeometry() const;

				// Feature access, based on https://github.com/pelias/schema/blob/master/mappings/document.js

				const char* Layer() const;

				const char* Name() const;
				const char* Text() const; // same as Phrase in the schema ?

				const char* Address() const;
				const char* Alpha3() const;

				const char* Admin0() const;
				const char* Admin1() const;
				const char* Admin1_abbr() const;
				const char* Admin2() const;
				const char* LocalAdmin() const;

				const char* Locality() const;
				const char* Neighborhood() const;

				const Geometry& CenterPoint() const;

				// how to represent Shape ?

				unsigned int CategoryCount() const;
				const char* Category(unsigned int index) const;
				// maybe add CategoryId to work with enum-like categories ?

				std::string Stringify() const;
			};

		}

	}
}

#endif
