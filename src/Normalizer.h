#ifndef Mapzen_Pelopia_Normalizer_h
#define Mapzen_Pelopia_Normalizer_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Internal interface for address normalization 
*/

#include <cstring>
#include <vector>

namespace Mapzen
{
	namespace Pelopia
	{
        class Normalizer
        {
        public:
            typedef struct {
                size_t startBytes;      // original term, offset from the start of the phrase
                size_t lengthBytes;     // original term, size up to and excluding the following punctuation/white space
                const char32_t* norm;   // NUL-terminated normalized term
            } NormalizedTerm;
            
            typedef std::vector < NormalizedTerm > Result;
            
        public:
            virtual ~Normalizer() {}
            
            // phrase is in UTF-8-encoded Unicode
            // the returned reference stays valid until the next call to Normalize
            virtual const Result& Normalize ( const char* phrase, size_t sizeBytes ) = 0;
            
        protected:
            Result m_result;
        };
    }
}

#endif

