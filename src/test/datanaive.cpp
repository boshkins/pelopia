#include "../DataNaive.h"

#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace Mapzen :: Pelopia :: GeocodeJSON;
using namespace std;

TEST_CASE ( "DataNaive Construct from a non-existent file" ) 
{
    REQUIRE_THROWS_AS ( DataNaive ds ( "./input/notthere" ), invalid_argument );
}

TEST_CASE ( "DataNaive Construct from an empty file" ) 
{
    REQUIRE_THROWS_AS ( DataNaive ds ( "./input/empty.json" ), logic_error );
}

TEST_CASE ( "DataNaive Construct, Count" ) 
{
    DataNaive ds ( "./input/one-term.json" );
    REQUIRE ( 10 == ds . Count() );
}
#if 0
TEST_CASE ( "DataNaive Construct, Get out of bounds" ) 
{
    DataNaive ds ( "./input/one-term.json" );
    SECTION ( "Id > Count") { REQUIRE_THROWS_AS ( ds . Get ( 11 ), invalid_argument ); }
    SECTION ( "Id < 1")     { REQUIRE_THROWS_AS ( ds . Get ( 0 ), invalid_argument ); }
}

TEST_CASE ( "DataNaive Construct, Get" ) 
{
    DataNaive ds ( "./input/one-term.json" );
    const Feature& f = ds.Get ( 3 );
    // check only selected properties, the ones that conform to GeocodeJSON spec
    //TODO: make sure all properties in the test files confrom to the spec; add check here     
    REQUIRE ( string ( "address" )                                  == f.Layer () ); 
    REQUIRE ( string ( "oa" )                                       == f.Source () ); 
    REQUIRE ( 0.597                                                 == f.Confidence () ); 
    REQUIRE ( string ( "3 Warwickshire Road, Warwick, Bermuda" )    == f.Label () ); 
    REQUIRE ( string ( "3 Warwickshire Road" )                      == f.Name () ); 
    REQUIRE ( string ( "3" )                                        == f.HouseNumber () ); 
    REQUIRE ( string ( "Warwickshire Road" )                        == f.Street () ); 
    REQUIRE ( string ( "Warwick" )                                  == f.Region () ); 
    REQUIRE ( string ( "Bermuda" )                                  == f.Country () );
    
    const Geometry* g = f.GetGeometry ();
    REQUIRE ( nullptr != g );
    REQUIRE ( -64.810874 == g->Longitude ());
    REQUIRE ( 32.260458  == g->Latitude ());
}
#endif