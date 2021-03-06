#ifndef Mapzen_Pelopia_TextScorer_h
#define Mapzen_Pelopia_TextScorer_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Feature Scorer with text scoring
*/

#include "Scorer.h"

namespace Mapzen
{
	namespace Pelopia
	{
        class TextScorer : public Scorer
        {
        public:
            TextScorer ( const Dataset &, const Normalizer&, const Normalizer :: Result & query );
            virtual ~TextScorer();

            virtual MatchQuality Score ( Id ) const;

        protected:
            double TermFrequency ( const Normalizer::Result& address, const char32_t*  term ) const;
            double InverseDocumentFrequency ( const char32_t*  term ) const;
            double TermWeight ( const Normalizer::Result& address, const char32_t*  term ) const;
            double QueryNormalization () const;
            double QueryCoordination ( const Normalizer::Result& address ) const;

        private:
            const Normalizer&           m_norm;
            const Normalizer :: Result& m_query;

            double m_queryNormFactor;
        };
    }
}

#endif

