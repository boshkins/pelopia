#ifndef Mapzen_Pelopia_GeocodeJSON_h
#define Mapzen_Pelopia_GeocodeJSON_h

/*
* Copyright notice: TBD
*/

/*
* Pelopia :: GeocodeJSON, declarations related to GeocodeJSON objects
*/

#include <pelopia/Pelopia.h>

#include <string>

namespace Mapzen
{
	namespace Pelopia
	{
		/**
		* GeocodeJSON declarations, based on https://github.com/pelias/geocodejson-spec
		*/
		namespace GeocodeJSON {

			/**
			* GeocodeJSON Geometry. For now, only Point in Geographic Coordinate Reference System
			*/
			class Geometry
			{	//  
			public:
				Coordinate Latitude() const;
				Coordinate Longitude() const;
			};

			/**
			* GeocodeJSON Feature object corresponding to a single Location
			*/
			class Feature
			{
			public:
				const Geometry* GetGeometry() const;

				// Access to Feature object's properties, based on 
                // https://github.com/pelias/geocodejson-spec/tree/master/draft
                // optional propreties return 0/NULL if omitted
                
                //TBD: enum PropertyID for generic access to (string) properties

				const char* Id () const;
				const char* Layer () const;

				const char* Source () const; // optional
				uint32_t AccuracyMeters () const; // optional
				double Confidence () const; // 0.0 .. 1.0
                
				const char* Label () const; 
				const char* Name () const; // "recommended" but still optional

				const char* HouseNumber () const; // optional
				const char* Street () const; // optional
				const char* Postcode () const; // optional
				const char* City () const; // optional
				const char* District () const; // optional
				const char* County () const; // optional
				const char* Region () const; // optional
				const char* RegionAbbr () const; // optional
				const char* Country () const; // optional
				const char* CountryAbbr () const; // optional

				const char* Admin ( unsigned int level ) const; // optional; level from 1 to 10

				const char* Geohash () const; // optional

				const Geometry& CenterPoint() const;

				std::string Stringify() const;
			};

		}

	}
}

#endif
