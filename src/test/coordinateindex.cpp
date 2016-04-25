#include "../CoordinateIndex.h"

#include <test/catch.hpp>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "CoordinateIndex populate" )
{
    CoordinateIndex idx;
    idx.Insert ( 1.1, 1 );
    idx.Insert ( 2.2, 2 );

    SECTION( "ConstIterator" )
    {
        CoordinateIndex::ConstIterator it1 = idx.FindSegment ( 0, 10 );

        SECTION ( "!=" )
        {
            REQUIRE ( it1 != idx.end() );
        }
        SECTION ( "copy construction, ==" )
        {
            CoordinateIndex::ConstIterator it2 = it1;
            REQUIRE ( it1 == it2 );
        }
        SECTION("increment")
        {
            FAIL("not implemented");
        }
    }

    SECTION ("end()")
    {
        FAIL("not implemented");
    }
}
