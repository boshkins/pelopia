#ifndef Mapzen_Pelopia_GeoTextScorer_h
#define Mapzen_Pelopia_GeoTextScorer_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Feature Scorer with geo distance and text scoring.
*/

#include <pelopia/Pelopia.h>

#include "TextScorer.h"
#include "GeoScorer.h"

namespace Mapzen
{
	namespace Pelopia
	{
        class LocationFilter;
        class LatLon;

        class GeoTextScorer : public Scorer
        {
        public:
            GeoTextScorer ( const Dataset &                 dataset,
                            const LatLon &                  center,
                            const Normalizer &              normalizer,
                            const Normalizer :: Result &    query );
            virtual ~GeoTextScorer();

            virtual MatchQuality Score ( Id ) const;

        private:
            GeoScorer   m_geo;
            TextScorer  m_text;
        };
    }
}

#endif

