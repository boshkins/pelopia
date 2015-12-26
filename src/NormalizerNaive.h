#ifndef Mapzen_Pelopia_NormalizerNaive_h
#define Mapzen_Pelopia_NormalizerNaive_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Internal interface for address normalization 
*/

#include "Normalizer.h"

namespace Mapzen
{
	namespace Pelopia
	{
        class NormalizerNaive : public Normalizer
        {
        public:
            NormalizerNaive();
            virtual ~NormalizerNaive();
            
            virtual const Result& Normalize ( const char* phrase, size_t sizeBytes );
            
        private:
            std::vector < char32_t > m_strings;
        };
    }
}

#endif

