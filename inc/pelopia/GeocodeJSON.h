#ifndef Mapzen_Pelopia_GeocodeJSON_h
#define Mapzen_Pelopia_GeocodeJSON_h

/*
* Copyright notice: TBD
*/

/*
* Pelopia :: GeocodeJSON, declarations related to GeocodeJSON objects
*/

#include <pelopia/BoundingBox.h>

#include <string>
#include <stdexcept>

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
			{	  
			public:
                Geometry () : m_lat ( 0 ), m_lon ( 0 )
                {
                }
                Geometry ( Coordinate p_lat, Coordinate p_lon ) : m_lat ( p_lat ), m_lon ( p_lon )
                {
                }
                
				Coordinate Latitude() const { return m_lat; }
				Coordinate Longitude() const { return m_lon; }
                
                bool operator == ( const Geometry& p_that ) const 
                {
                    return m_lat == p_that . m_lat && m_lon == p_that . m_lon;
                }
                bool operator != ( const Geometry& p_that ) const 
                {
                    return ! ( *this == p_that );
                }
                
				void SetLatitude ( Coordinate p_lat ) { m_lat = p_lat; }
				void SetLongitude ( Coordinate p_lon ) { m_lon = p_lon; }
                
            private:
                Coordinate m_lat;
                Coordinate m_lon;
			};

			/**
			* GeocodeJSON Feature object corresponding to a single Location
			*/
			class Feature
			{
            public:
                // generic access to searchable properties
                typedef enum 
                {
                    Property_begin = 0,
                    
                    Property_Label = Property_begin,
                    Property_Name,
                    Property_HouseNumber,
                    Property_Street,
                    Property_Postcode,
                    Property_City,
                    Property_District,
                    Property_County,
                    Property_Region,
                    Property_RegionAbbr,
                    Property_Country,
                    Property_CountryAbbr,
                    Property_Geohash,
                    
                    Property_end
                } SearchablePropertyId;
                
            public:
                virtual ~Feature() = 0;
                
			public:
                virtual const BoundingBox* GetBoundingBox() const = 0; // optional
				virtual const Geometry* GetGeometry() const = 0;

				// Access to Feature object's properties, based on 
                // https://github.com/pelias/geocodejson-spec/tree/master/draft
                // optional propreties return 0/nullptr if omitted
                
				virtual const char* Id () const = 0;
				virtual const char* Layer () const = 0;
                
				virtual const char* Source () const = 0; // optional
				virtual uint32_t AccuracyMeters () const = 0; // optional
				virtual double Confidence () const = 0; // 0.0 .. 1.0; optional
                
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
                
                const char* SearchableProperty ( SearchablePropertyId ) const throw ( std :: invalid_argument );
                
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
