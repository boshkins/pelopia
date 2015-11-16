#include "../DataNaive.h"

#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "DataNaive Construct from a non-existent file" ) 
{
    REQUIRE_THROWS_AS ( DataNaive ds ( "./input/notthere" ), invalid_argument );
}

TEST_CASE ( "DataNaive Construct from an empty file" ) 
{
    REQUIRE_THROWS_AS ( DataNaive ds ( "./input/empty.json" ), invalid_argument );
}

