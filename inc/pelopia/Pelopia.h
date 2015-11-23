#ifndef Mapzen_Pelopia_h
#define Mapzen_Pelopia_h

/*
* Copyright notice: TBD
*/

/*
* API for Pelopia, an open source offline geocoding library
* Project repository: https://github.com/boshkins/pelopia
*/

#include <cstdint>
#include <stdexcept>

namespace Mapzen
{
	namespace Pelopia
	{

		typedef uint32_t ResultCount;

		typedef struct 
		{
			ResultCount maxResults;
		} Format;
		const ResultCount AllResults = 0;
		const ResultCount DefaultResults = 10;

		typedef double Coordinate; // degrees; underlying datatype is likely to change

        // class LatLon:
        // Latitude in the range of -90 .. 90
        // Longitude in the range -180 .. 540; the extended range may be used to handle discontinuity
        //                                      at the 180 meridian
        // the constructor and setters will throw if latitude / longitude is outside of its range
		class LatLon 
		{
        public:
			LatLon (); // 0, 0  
			LatLon ( Coordinate p_lat, Coordinate p_lon ) throw ( std :: logic_error ); 

			Coordinate Latitude  () const { return m_lat; }
			Coordinate Longitude () const { return m_lon; }
            
			void SetLatitude  ( Coordinate )  throw ( std :: logic_error );
			void SetLongitude ( Coordinate ) throw ( std :: logic_error );

            bool operator == ( const LatLon& p_that ) const 
            {
                return m_lat == p_that . m_lat && m_lon == p_that . m_lon;
            }
            bool operator != ( const LatLon& p_that ) const 
            {
                return ! ( *this == p_that );
            }
            
        private:
			Coordinate m_lat;	
			Coordinate m_lon; 
        };

		typedef uint64_t Id;
		const Id InvalidId = 0;

		// Match score, in the range 0 .. 1
		typedef double MatchQuality;

	}
}

#endif
