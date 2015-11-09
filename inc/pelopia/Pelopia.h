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

		struct LatLon 
		{
			LatLon ( Coordinate p_lat, Coordinate p_lon );

			Coordinate lat;	// range -180 .. 180 
			Coordinate lon; // range -180 .. 360; the extended range is used to handle discontinuity
		};					//  at the 180 meridian

		typedef uint64_t Id;
		const Id InvalidId = 0;

		// Match score, in the range 0 .. 1
		typedef double MatchQuality;

	}
}

#endif
