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

TextScorer::TextScorer ( const Dataset & p_dataset, const Normalizer& p_norm, const Normalizer :: Result & p_query )
:   Scorer ( p_dataset ),
    m_norm  (p_norm ),
    m_query ( p_query ),
    m_queryNormFactor ( 0 )
{
    // QUERY_NORM = 1 / sqrt ( sum ( IDF of each term in the query ) )
    double totalIDF = 0;
    for ( auto&& i : m_query.m_terms )
    {
        totalIDF += InverseDocumentFrequency ( i.norm );
    }
    if ( totalIDF != 0 )
    {
        m_queryNormFactor = 1.0 / sqrt ( totalIDF );
    }
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
TextScorer::TermFrequency ( const Normalizer::Result& p_address, const char32_t* p_term ) const
{
    size_t count = 0;
    for ( auto&& f : p_address.m_terms )
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
    // Call at construction time

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
    return TermFrequency ( p_address, p_term ) *
           InverseDocumentFrequency ( p_term ) *
           ( 1.0 / sqrt ( double ( p_address.m_terms.size() ) ) );
}

double
TextScorer::QueryNormalization () const
{   // QUERY_NORM = 1 / sqrt ( sum ( IDF of each term in the query ) )
    return m_queryNormFactor;
}

double
TextScorer::QueryCoordination ( const Normalizer::Result& p_address ) const
{   // QueryCoordination = # of matching terms in the field / total terms in the query
    if ( m_query.m_terms.size () == 0 )
    {
        return 0;
    }

    unsigned int matchCount = 0;
    for ( auto&& i : m_query.m_terms )
    {
        for  (auto&& f : p_address.m_terms )
        {
            if ( MatchChar32 ( f.norm, i.norm ) )
            {
                ++matchCount;
            }
        }
    }
    return (double)matchCount / m_query.m_terms.size ();
}

MatchQuality
TextScorer::Score ( Id p_id ) const
{   // Score = QueryNormalizationFactor * QueryCoordination * sum ( TermWeight, for every term )
    Normalizer::Result normAddress;
    m_norm.Normalize ( GetDataset().Place( p_id ).Label(), normAddress );
    double totalWeight = 0;
    for ( auto&& i : m_query.m_terms )
    {
        //TODO. Eliminate multiple calculations of each term's IDF (once in constructor, once here inside TermWeight)
        totalWeight += TermWeight ( normAddress, i.norm );
    }
    return  m_queryNormFactor * QueryCoordination ( normAddress ) * totalWeight;
}
