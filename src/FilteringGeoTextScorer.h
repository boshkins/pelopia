#ifndef Mapzen_Pelopia_FilteringGeoTextScorer_h
#define Mapzen_Pelopia_FilteringGeoTextScorer_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Feature Scorer with geo distance and text scoring, and externally provided filter.
*/

#include "GeoTextScorer.h"

namespace Mapzen
{
	namespace Pelopia
	{
        class LocationFilter;
        class LatLon;

        class FilteringGeoTextScorer : public GeoTextScorer
        {
        public:
            FilteringGeoTextScorer ( const Dataset &,
                                     const LocationFilter&,
                                     const LatLon & p_center,
                                     const Normalizer :: Result & query );
            virtual ~FilteringGeoTextScorer();

            virtual MatchQuality Score ( Id ) const; // 0 - filtered out
        };
    }
}

#endif

