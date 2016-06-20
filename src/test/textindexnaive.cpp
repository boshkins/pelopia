#include "../TextIndexNaive.h"

#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "TestIndexNaiveWriter Construct" )
{
    TextIndexNaiveWriter w;
}

TEST_CASE ( "TestIndexNaiveWriter-Reader empty index" )
{
    TextIndexNaiveWriter w;
    TextIndexNaiveReader r ( w.DetachIndex() );
    REQUIRE ( nullptr == r.Locate(U"term") );
}

TEST_CASE ( "TestIndexNaiveWriter-Reader Add/Locate, one term" )
{
    TextIndexNaiveWriter w;
    const string term1 = "term1";
    w.AddTermUse ( term1,
                   1,
                   GeocodeJSON :: Feature :: Property_Name,
                   2,
                   3);

    TextIndexNaiveReader r (w.DetachIndex());

    SECTION ("full term search")
    {
        const TextIndexReader::Node* node = r.Locate(U"term1");
        REQUIRE ( nullptr != node );
        REQUIRE ( 0 == node->ChildrenCount() );

        const TextIndexReader::Payload& p = node->GetPayload();
        REQUIRE( 1 == p.Count() );
        REQUIRE( 1 == p.FeatureCount() );

        const TextIndexReader::TermUse& use = p.GetTermUse ( 0 );
        REQUIRE( 1 == use.ObjectId() );
        REQUIRE( GeocodeJSON :: Feature :: Property_Name == use.PropertyId() );
        size_t index;
        size_t offset;
        size_t length;
        use.Position ( index, offset, length );
        REQUIRE ( 2 == index );
        REQUIRE ( 3 == offset );
        REQUIRE ( term1. size() == length );
    }
}

TEST_CASE ( "TestIndexNaiveWriter-Reader Add/Locate, FeatureCount" )
{
    TextIndexNaiveWriter w;
    const string term1 = "term1";
    w.AddTermUse ( term1, 1, GeocodeJSON :: Feature :: Property_Name, 2, 3);
    w.AddTermUse ( term1, 1, GeocodeJSON :: Feature :: Property_Name, 2, 3);
    TextIndexNaiveReader r (w.DetachIndex());

    const TextIndexReader::Payload& p = r.Locate(U"term1")->GetPayload();
    REQUIRE( 2 == p.Count() );
    REQUIRE( 1 == p.FeatureCount() );
}

//TODO: UTF-8 encoded Unicode strings
//TODO: support array properties (TextIndexWriter::Add with index etc.)
//TODO: sort order in Payload::GetTermUse ()
