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
            FilteringGeoTextScorer ( const Dataset &                p_dataset,
                                     const LocationFilter&          p_filter,
                                     const LatLon &                 p_center,
                                     const Normalizer &             p_normalizer,
                                     const Normalizer :: Result &   p_query );
            virtual ~FilteringGeoTextScorer();

            virtual MatchQuality Score ( Id ) const; // 0 - filtered out

        private:
            const LocationFilter& m_filter;
        };
    }
}

#endif

