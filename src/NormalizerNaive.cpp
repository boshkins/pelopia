#include "NormalizerNaive.h"

#include <cwctype>

#include "utf8cpp/utf8.h"

using namespace Mapzen :: Pelopia;
using namespace std;

NormalizerNaive :: NormalizerNaive ()
{
}

NormalizerNaive :: ~NormalizerNaive()
{
}

const Normalizer :: Result&
NormalizerNaive :: Normalize ( const char* p_phrase )
{
    m_result.clear();
    m_strings.clear();
    size_t sizeBytes = strlen ( p_phrase );
    m_strings.reserve ( sizeBytes + 1 ); // enough for the "worst" case: 1-term phrase

    const char* ch = p_phrase;
    const char* end = p_phrase + sizeBytes;
    const char* curTerm = nullptr;
    size_t curNormTerm = 0;

    while ( ch < end )
    {
        const char* prev_ch = ch;
        uint32_t cp = utf8 :: next ( ch, end );

        if ( ::iswpunct ( cp ) || ::iswspace ( cp ) )
        {
            if ( curTerm == nullptr )
            {   // skip whitespace/punctuation
                continue;
            }
            else
            {   // save the new term
                m_strings.push_back ( 0 );

                NormalizedTerm nt;
                nt.startBytes = curTerm - p_phrase;
                nt.lengthBytes = prev_ch - curTerm;
                nt.norm = m_strings.data() + curNormTerm;

                m_result.push_back ( nt );
                curTerm = nullptr;
            }
        }
        else
        {   // not whitespace/punctuation
            if ( curTerm == nullptr )
            {   // a new term starts
                curTerm = prev_ch;
                curNormTerm = m_strings.size();
            }
            m_strings.push_back ( cp );
        }

    }

    if ( curTerm != nullptr )
    {   // handle the last term
        m_strings.push_back ( 0 );

        NormalizedTerm nt;
        nt.startBytes = curTerm - p_phrase;
        nt.lengthBytes = end - curTerm;
        nt.norm = m_strings.data() + curNormTerm;

        m_result.push_back(nt);
    }

    return m_result;
}
