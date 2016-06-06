#include "../NormalizerNaive.h"

#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "NormalizerNaive Construct" )
{
    NormalizerNaive norm;
}

TEST_CASE ( "NormalizerNaive Empty phrase" )
{
    NormalizerNaive norm;
    Normalizer::Result res;
    norm.Normalize ( "", res );
    REQUIRE ( 0 == res.m_terms.size() );
}

TEST_CASE ( "NormalizerNaive One term phrase" )
{
    NormalizerNaive norm;
    Normalizer::Result res;
    const char* term = "term";
    norm.Normalize ( term, res );
    REQUIRE ( 1 == res.m_terms.size() );
    REQUIRE ( res.m_terms [ 0 ].startBytes == 0 );
    REQUIRE ( res.m_terms [ 0 ].lengthBytes == strlen ( term ) );
    REQUIRE ( u32string ( res.m_terms [ 0 ].norm ) == U"term" );
}

TEST_CASE ( "NormalizerNaive Multi term phrase" )
{
    NormalizerNaive norm;
    Normalizer::Result res;
    const char* phrase = "term1 term2,term3;      term4";
    norm.Normalize ( phrase, res );
    REQUIRE ( 4 == res.m_terms.size() );

    REQUIRE ( res.m_terms [ 0 ].startBytes == 0 );
    REQUIRE ( res.m_terms [ 0 ].lengthBytes == 5 );
    REQUIRE ( u32string ( res.m_terms [ 0 ].norm ) == U"term1" );

    REQUIRE ( res.m_terms [ 1 ].startBytes == 6 );
    REQUIRE ( res.m_terms [ 1 ].lengthBytes == 5 );
    REQUIRE ( u32string ( res.m_terms [ 1 ].norm ) == U"term2" );

    REQUIRE ( res.m_terms [ 2 ].startBytes == 12 );
    REQUIRE ( res.m_terms [ 2 ].lengthBytes == 5 );
    REQUIRE ( u32string ( res.m_terms [ 2 ].norm ) == U"term3" );

    REQUIRE ( res.m_terms [ 3 ].startBytes == 24 );
    REQUIRE ( res.m_terms [ 3 ].lengthBytes == 5 );
    REQUIRE ( u32string ( res.m_terms [ 3 ].norm ) == U"term4" );
}

TEST_CASE ( "NormalizerNaive Multi term Unicode" )
{
    NormalizerNaive norm;
    Normalizer::Result res;

    const char* phrase = "На берегу пустынных волн";
    norm.Normalize ( phrase, res );
    REQUIRE ( 4 == res.m_terms.size() );

    REQUIRE ( res.m_terms [ 0 ].startBytes == 0 );
    REQUIRE ( res.m_terms [ 0 ].lengthBytes == 4 );
    REQUIRE ( u32string ( res.m_terms [ 0 ].norm ) == U"На" );

    REQUIRE ( res.m_terms [ 1 ].startBytes == 5 );
    REQUIRE ( res.m_terms [ 1 ].lengthBytes == 12 );
    REQUIRE ( u32string ( res.m_terms [ 1 ].norm ) == U"берегу" );

    REQUIRE ( res.m_terms [ 2 ].startBytes == 18 );
    REQUIRE ( res.m_terms [ 2 ].lengthBytes == 18 );
    REQUIRE ( u32string ( res.m_terms [ 2 ].norm ) == U"пустынных" );

    REQUIRE ( res.m_terms [ 3 ].startBytes == 37 );
    REQUIRE ( res.m_terms [ 3 ].lengthBytes == 8 );
    REQUIRE ( u32string ( res.m_terms [ 3 ].norm ) == U"волн" );
}

