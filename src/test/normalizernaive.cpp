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
    REQUIRE ( 0 == norm.Normalize ( "" ).size() );
}

TEST_CASE ( "NormalizerNaive One term phrase" )
{
    NormalizerNaive norm;
    const char* term = "term";
    Normalizer::Result res = norm.Normalize ( term );
    REQUIRE ( 1 == res.size() );
    REQUIRE ( res [ 0 ].startBytes == 0 );
    REQUIRE ( res [ 0 ].lengthBytes == strlen ( term ) );
    REQUIRE ( u32string ( res [ 0 ].norm ) == U"term" );
}

TEST_CASE ( "NormalizerNaive Multi term phrase" )
{
    NormalizerNaive norm;
    const char* phrase = "term1 term2,term3;      term4";
    Normalizer::Result res = norm.Normalize ( phrase );
    REQUIRE ( 4 == res.size() );

    REQUIRE ( res [ 0 ].startBytes == 0 );
    REQUIRE ( res [ 0 ].lengthBytes == 5 );
    REQUIRE ( u32string ( res [ 0 ].norm ) == U"term1" );

    REQUIRE ( res [ 1 ].startBytes == 6 );
    REQUIRE ( res [ 1 ].lengthBytes == 5 );
    REQUIRE ( u32string ( res [ 1 ].norm ) == U"term2" );

    REQUIRE ( res [ 2 ].startBytes == 12 );
    REQUIRE ( res [ 2 ].lengthBytes == 5 );
    REQUIRE ( u32string ( res [ 2 ].norm ) == U"term3" );

    REQUIRE ( res [ 3 ].startBytes == 24 );
    REQUIRE ( res [ 3 ].lengthBytes == 5 );
    REQUIRE ( u32string ( res [ 3 ].norm ) == U"term4" );
}

TEST_CASE ( "NormalizerNaive Multi term Unicode" )
{
    NormalizerNaive norm;

    const char* phrase = "На берегу пустынных волн";
    Normalizer::Result res = norm.Normalize ( phrase );
    REQUIRE ( 4 == res.size() );

    REQUIRE ( res [ 0 ].startBytes == 0 );
    REQUIRE ( res [ 0 ].lengthBytes == 4 );
    REQUIRE ( u32string ( res [ 0 ].norm ) == U"На" );

    REQUIRE ( res [ 1 ].startBytes == 5 );
    REQUIRE ( res [ 1 ].lengthBytes == 12 );
    REQUIRE ( u32string ( res [ 1 ].norm ) == U"берегу" );

    REQUIRE ( res [ 2 ].startBytes == 18 );
    REQUIRE ( res [ 2 ].lengthBytes == 18 );
    REQUIRE ( u32string ( res [ 2 ].norm ) == U"пустынных" );

    REQUIRE ( res [ 3 ].startBytes == 37 );
    REQUIRE ( res [ 3 ].lengthBytes == 8 );
    REQUIRE ( u32string ( res [ 3 ].norm ) == U"волн" );
}

