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
        class ScorerWithDistance : public Scorer
        {
        public:
            ScorerWithDistance ( const Dataset & ); 
            virtual ~ScorerWithDistance();
            
            void SetCoordinates ( const LatLon & p_coords ) { m_coords = p_coords; }
            const LatLon & GetCoordinates () const { return m_coords; }
            
            virtual MatchQuality Score ( const GeocodeJSON::Feature & ) const;
            
        protected:
            MatchQuality GeoScore ( const Distance& p_fromCenter ) const;
            MatchQuality TextScore () const;        
            
        private:
            const Dataset&  m_dataset;
            LatLon          m_coords;
            bool            m_coordsSet;
            double          m_lambda;
        };
    }
}

#endif

