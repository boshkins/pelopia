#ifndef Mapzen_Pelopia_ScorerWithExpDistance_h
#define Mapzen_Pelopia_ScorerWithExpDistance_h

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
            TextScorer ( const Dataset &, const Normalizer :: Result & query );
            virtual ~TextScorer();

            virtual MatchQuality Score ( Id ) const;

        protected:
            double TermFrequency ( const Normalizer::Result& address, const char32_t* ) const;
            double InverseDocumentFrequency ( const char32_t* ) const;
            double TermWeight ( const Normalizer::Result& address, const char32_t* ) const;

        private:
            const Normalizer :: Result& m_query;
        };
    }
}

#endif

