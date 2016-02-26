#ifndef Mapzen_Pelopia_ScorerWithExpDistance_h
#define Mapzen_Pelopia_ScorerWithExpDistance_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Feature Scorer with text and distance scoring 
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
            
            const LatLon & GetCenter () const { return m_center; }
            
            virtual MatchQuality Score ( const GeocodeJSON::Feature & ) const;
            
        private:
            const Dataset&  m_dataset;
            LatLon          m_center;
            const double    m_lambda;
        };
    }
}

#endif

