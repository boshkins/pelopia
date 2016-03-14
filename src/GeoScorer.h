#ifndef Mapzen_Pelopia_GeoScorer_h
#define Mapzen_Pelopia_GeoScorer_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Feature Scorer with geo distance scoring
*/

#include "Scorer.h"

namespace Mapzen
{
	namespace Pelopia
	{
        class GeoScorer : public Scorer
        {
        public:
            GeoScorer ( const Dataset &, const LatLon & p_center );
            virtual ~GeoScorer();

            virtual MatchQuality Score ( Id ) const;

        private:
            LatLon          m_center;
            const double    m_lambda;
        };
    }
}

#endif

