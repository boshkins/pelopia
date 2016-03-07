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

            virtual MatchQuality Score ( Id ) const = 0;

        protected:
            Scorer ( const Dataset& p_dataset ) : m_dataset ( p_dataset ) {}

            const Dataset& GetDataset() const { return m_dataset; }

        private:
            const Dataset& m_dataset;
        };
    }
}

#endif

