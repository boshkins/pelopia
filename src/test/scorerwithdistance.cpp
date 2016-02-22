#include "../ScorerWithDistance.h"

#include "../NormalizerNaive.h"


#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "ScorerWithDistance Construct" ) 
{
    Dataset ds ( "input/one-term.json" );
    ScorerWithDistance s ( ds );
    
    NormalizerNaive norm;
    string term = "Warwickshire";
    Normalizer::Result res = norm.Normalize ( term.data(), term.size() );
    
    s.SetQuery ( res );
    
    REQUIRE ( U"Warwickshire" == u32string ( (*s.GetQuery())[0].norm ) ); 
         
    LatLon focus ( -81.598765, 30.21615 );
    s.SetCoordinates ( focus );
    
    REQUIRE ( focus == s.GetCoordinates() );
}

TEST_CASE ( "ScorerWithDistance ScoreNoDistance" ) 
{
    Dataset ds ( "input/one-term.json" );
    ScorerWithDistance s ( ds );
    
    NormalizerNaive norm;
    string term = "Warwickshire";
    Normalizer::Result res = norm.Normalize ( term.data(), term.size() );
    s.SetQuery ( res );
    
    REQUIRE ( 0.5 == s.Score ( ds.Place ( 1 ) ) ); 
}

