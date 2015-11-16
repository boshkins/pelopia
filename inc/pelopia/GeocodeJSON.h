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
				virtual const Geometry* GetGeometry() const = 0;

				// Access to Feature object's properties, based on 
                // https://github.com/pelias/geocodejson-spec/tree/master/draft
                // optional propreties return 0/NULL if omitted
                
                //TBD: enum PropertyID for generic access to (string) properties

				virtual const char* Id () const = 0;
				virtual const char* Layer () const = 0;
                
				virtual const char* Source () const = 0; // optional
				virtual uint32_t AccuracyMeters () const = 0; // optional
				virtual double Confidence () const = 0; // 0.0 .. 1.0
                
				virtual const char* Label () const = 0; 
				virtual const char* Name () const = 0; // "recommended" but still optional
                
				virtual const char* HouseNumber () const = 0; // optional
				virtual const char* Street () const = 0; // optional
				virtual const char* Postcode () const = 0; // optional
				virtual const char* City () const = 0; // optional
				virtual const char* District () const = 0; // optional
				virtual const char* County () const = 0; // optional
				virtual const char* Region () const = 0; // optional
				virtual const char* RegionAbbr () const = 0; // optional
				virtual const char* Country () const = 0; // optional
				virtual const char* CountryAbbr () const = 0; // optional
                
				virtual const char* Admin ( unsigned int level ) const = 0; // optional; level from 1 to 10
                
				virtual const char* Geohash () const = 0; // optional
                
				virtual const Geometry& CenterPoint() const = 0;

				virtual std::string Stringify() const = 0;
                
                // create a copy of this object, to be deleted by the caller
                virtual Feature* Clone() const = 0;
                
            private:
                Feature& operator= ( const Feature& );
			};

		}

	}
}

#endif
