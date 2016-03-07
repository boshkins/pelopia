/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Implementation of Feature Scorer with text scoring
*/

#include "TextScorer.h"
#include "TextIndex.h"

#include <cmath>

using namespace Mapzen::Pelopia;

TextScorer::TextScorer ( const Dataset & p_dataset, const Normalizer :: Result & p_query )
:   Scorer ( p_dataset ),
    m_query ( p_query )
{
}

TextScorer::~TextScorer ()
{
}

static
bool
MatchChar32 ( const char32_t* p_a, const char32_t* p_b )
{
    while ( *p_a != 0 )
    {
        if ( *p_a != *p_b )
        {
            return false;
        }
        ++p_a;
        ++p_b;
    }
    return *p_b == 0;
}

double
TextScorer::TermFrequency ( const Normalizer::Result& p_field, const char32_t* p_term ) const
{
    size_t count = 0;
    for ( auto&& f : p_field )
    {
        if ( MatchChar32 ( f.norm, p_term ) )
        {
            ++ count;
        }
    }
    switch ( count )
    {
        case 0: return 0.0;
        case 1: return 1.0;
        default: return sqrt ( count ); //TODO: use a table
    }
}

double
TextScorer::InverseDocumentFrequency ( const char32_t* p_term ) const
{   // IDF = 1 + log ( totalFeatures / ( 1 + # of features containing the term ) )
    const TextIndexReader::Node* node = GetDataset().GetIndex().Locate ( p_term );
    unsigned int featuresContaining = 0;
    if ( node != nullptr )
    {
        featuresContaining = node->GetPayload().FeatureCount();
    }
    return 1.0 + log ( double ( GetDataset().FeatureCount() ) / double ( 1 + featuresContaining ) );
}

double
TextScorer::TermWeight ( const Normalizer::Result& p_address, const char32_t* p_term ) const
{   // TermWeight = TF * IDF * ( 1 / sqrt # of terms in the address )
    return TermFrequency ( p_address, p_term ) * InverseDocumentFrequency ( p_term ) * ( 1.0 / sqrt ( double ( p_address.size() ) ) );
}


MatchQuality
TextScorer::Score ( Id ) const
{
    return 1.0;
}
