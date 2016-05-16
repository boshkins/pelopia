#include "../CoordinateIndex.h"

#include <test/catch.hpp>

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "CoordinateIndex" )
{
    SECTION( "ConstIterator basics" )
    {
        CoordinateIndex idx;
        idx.Insert ( 1.1, 1 );
        idx.Insert ( 2.2, 2 );

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
        SECTION("dereference")
        {
            REQUIRE ( *it1 == 1 );
        }
        SECTION("increment postfix")
        {
            CoordinateIndex::ConstIterator it2 = it1;
            it2++;
            REQUIRE ( *it2 == 2 );
        }
        SECTION("increment prefix")
        {
            CoordinateIndex::ConstIterator it2 = it1;
            ++it2;
            REQUIRE ( *it2 == 2 );
        }
        SECTION("end of iteration")
        {
            CoordinateIndex::ConstIterator it2 = it1;
            it2++;
            it2++;
            REQUIRE ( it2 == idx.end() );
        }
    }

    SECTION( "ConstIterator Search" )
    {
        CoordinateIndex idx;
        idx.Insert ( 1.1, 1 );
        idx.Insert ( 2.2, 2 );
        idx.Insert ( 3.3, 3 );

        SECTION ("empty 1")
        {
            CoordinateIndex::ConstIterator it = idx.FindSegment ( 0.0, 1.0 );
            REQUIRE ( it == idx.end() );
        }
        SECTION ("empty 2")
        {
            CoordinateIndex::ConstIterator it = idx.FindSegment ( 3.4, 10 );
            REQUIRE ( it == idx.end() );
        }
        SECTION ("empty 3")
        {
            CoordinateIndex::ConstIterator it = idx.FindSegment ( 2.0, 1.0 );
            REQUIRE ( it == idx.end() );
        }
        SECTION ("Partial 1")
        {
            CoordinateIndex::ConstIterator it = idx.FindSegment ( 0, 2.0 );
            REQUIRE ( it != idx.end() );
            REQUIRE ( *it == 1 );
            REQUIRE ( ++it == idx.end() );
        }
        SECTION ("Partial 2")
        {
            CoordinateIndex::ConstIterator it = idx.FindSegment ( 0, 3.0 );
            REQUIRE ( it != idx.end() );
            REQUIRE ( *it == 1 );
            REQUIRE ( ++it != idx.end() );
            REQUIRE ( *it == 2 );
            REQUIRE ( ++it == idx.end() );
        }
        SECTION ("Partial 3")
        {
            CoordinateIndex::ConstIterator it = idx.FindSegment ( 1.2, 2.3 );
            REQUIRE ( it != idx.end() );
            REQUIRE ( *it == 2 );
            REQUIRE ( ++it == idx.end() );
        }
        SECTION ("Partial 4")
        {
            CoordinateIndex::ConstIterator it = idx.FindSegment ( 1.2, 10.0 );
            REQUIRE ( it != idx.end() );
            REQUIRE ( *it == 2 );
            REQUIRE ( ++it != idx.end() );
            REQUIRE ( *it == 3 );
            REQUIRE ( ++it == idx.end() );
        }
        SECTION ("Partial 4")
        {
            CoordinateIndex::ConstIterator it = idx.FindSegment ( 2.3, 10.0 );
            REQUIRE ( it != idx.end() );
            REQUIRE ( *it == 3 );
            REQUIRE ( ++it == idx.end() );
        }
    }
}
