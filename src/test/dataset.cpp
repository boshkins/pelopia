#include <pelopia/Dataset.h>

#include <test/catch.hpp>

#include <stdexcept>

using namespace std;


using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "Dataset Construct from a non-existent file" ) 
{
    REQUIRE_THROWS_AS ( Dataset ds ( "./input/notthere" ), invalid_argument );
}

