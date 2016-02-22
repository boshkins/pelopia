#ifndef Mapzen_Pelopia_Scorer_h
#define Mapzen_Pelopia_Scorer_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Internal interface for scoring Geocode features against a (normalized) query 
*/

#include <pelopia/Dataset.h>
#include <pelopia/GeocodeJSON.h>

#include "Normalizer.h"

namespace Mapzen
{
	namespace Pelopia
	{
        class Scorer
        {
        public:
            virtual ~Scorer () {}
            
            void SetQuery ( const Normalizer::Result & p_query ) { m_query = & p_query; } 
            const Normalizer::Result* GetQuery () const { return m_query; }
            
            virtual MatchQuality Score ( const GeocodeJSON::Feature & ) const = 0;
            
        protected:
            Scorer () : m_query ( nullptr ) {}
            
        private:
            const Normalizer::Result* m_query;
        };
    }
}

#endif

