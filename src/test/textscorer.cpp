#include "../TextScorer.h"

#include "../NormalizerNaive.h"


#include <test/catch.hpp>

#include <stdexcept>

using namespace std;

using namespace Mapzen :: Pelopia;
using namespace std;

TEST_CASE ( "TextScorer One-term" )
{
    Dataset ds ( "input/one-term.json" );
    NormalizerNaive termNorm;

    class TestScorer : public TextScorer
    {
    public:
        TestScorer ( const Dataset & p_ds, const Normalizer :: Result & p_query ) : TextScorer ( p_ds, p_query ) {}
        using TextScorer::TermFrequency;
        using TextScorer::InverseDocumentFrequency;
        using TextScorer::TermWeight;
        using TextScorer::QueryCoordination;
    };
    NormalizerNaive norm;

    {
        Normalizer::Result normField = norm.Normalize ( ds.Place(1).Label() );

        // TF = sqrt ( # of apperances in the address ) = sqrt(1)
        SECTION ("TermFrequency, 1")
        {
            Normalizer::Result normQuery = termNorm.Normalize ("Warwickshire" );
            REQUIRE ( 1 == TestScorer ( ds, normQuery ).TermFrequency ( normField, normQuery [ 0 ].norm ) );
        }
        //TODO: add a section that produces something other than 0 or 1
        SECTION ("TermFrequency, 0")
        {
            Normalizer::Result normQuery = termNorm.Normalize ( "Sussex" );
            REQUIRE ( 0 == TestScorer ( ds, normQuery ).TermFrequency ( normField, normQuery [ 0 ].norm ) );
        }
    }

    // IDF = 1 + log ( totalFeatures / featuresContainingTheTerm ) = 1 + log ( 10 / 10 ) = 1
    SECTION ("InverseDocumentFrequency, very popular")
    {
        Normalizer::Result normQuery = termNorm.Normalize ("Warwickshire" );
        double idf = TestScorer ( ds, normQuery ).InverseDocumentFrequency ( normQuery [ 0 ].norm );
        REQUIRE ( 0.9046 == Approx ( idf ).epsilon ( 0.0001 ) );
    }
    SECTION ("InverseDocumentFrequency, less popular")
    {
        Normalizer::Result normQuery = termNorm.Normalize ( "Bermuda" );
        double idf = TestScorer ( ds, normQuery ).InverseDocumentFrequency ( normQuery [ 0 ].norm );
        REQUIRE ( 1.3566 == Approx ( idf ).epsilon ( 0.0001 ) );
    }
    SECTION ("InverseDocumentFrequency, absent")
    {
        Normalizer::Result normQuery = termNorm.Normalize ( "Sussex" );
        double idf = TestScorer ( ds, normQuery ).InverseDocumentFrequency ( normQuery [ 0 ].norm );
        REQUIRE ( 3.3025 == Approx ( idf ).epsilon ( 0.0001 ) );
    }

    // TermWeight = TF * IDF * ( 1 / sqrt # of terms in the address )
    SECTION ("TermWeight Id 1")
    {
        Normalizer::Result normField = norm.Normalize ( ds.Place(1).Label() ); // 3437 Warwickshire Road, Buckingham, PA
        Normalizer::Result normQuery = termNorm.Normalize ( "Warwickshire" );
        double tw = TestScorer ( ds, normQuery ).TermWeight ( normField, normQuery [ 0 ].norm );
        REQUIRE ( 0.4045 == Approx ( tw ).epsilon ( 0.0001 ) );
    }
    SECTION ("TermWeight Id 2")
    {
        Normalizer::Result normField = norm.Normalize ( ds.Place(2).Label() );  // 8 Warwickshire Road, Warwick, Bermuda
        Normalizer::Result normQuery = termNorm.Normalize ( "Bermuda" );
        double tw = TestScorer ( ds, normQuery ).TermWeight ( normField, normQuery [ 0 ].norm );
        REQUIRE ( 0.6067 == Approx ( tw ).epsilon ( 0.0001 ) ); // "Bermuda" is less frequent in the dataset than "Warwickshire", thus the score is higher than in the previous section
    }
    SECTION ("TermWeight, not found")
    {
        Normalizer::Result normField = norm.Normalize ( ds.Place(2).Label() );  // 8 Warwickshire Road, Warwick, Bermuda
        Normalizer::Result normQuery = termNorm.Normalize ( "Solovki" );
        double tw = TestScorer ( ds, normQuery ).TermWeight ( normField, normQuery [ 0 ].norm );
        REQUIRE ( 0.0 == tw ); // not in the address
    }

    SECTION ("QueryCoordination, all terms")
    {
        Normalizer::Result normField = norm.Normalize ( ds.Place(2).Label() );  // 8 Warwickshire Road, Warwick, Bermuda
        Normalizer::Result normQuery = termNorm.Normalize ( "Warwickshire Road Warwick Warwick" );
        REQUIRE ( 1 == TestScorer ( ds, normQuery ).QueryCoordination ( normField ) ); // all terms are in the address
    }
    SECTION ("QueryCoordination, some terms")
    {
        Normalizer::Result normField = norm.Normalize ( ds.Place(2).Label() );  // 8 Warwickshire Road, Warwick, Bermuda
        Normalizer::Result normQuery = termNorm.Normalize ( "Big House Warwick Bermuda" );
        REQUIRE ( 0.5 == TestScorer ( ds, normQuery ).QueryCoordination ( normField ) ); // 2 out of 4 terms are in the address
    }
    SECTION ("QueryCoordination, not in address")
    {
        Normalizer::Result normField = norm.Normalize ( ds.Place(2).Label() );  // 8 Warwickshire Road, Warwick, Bermuda
        Normalizer::Result normQuery = termNorm.Normalize ( "Solovki building 1" );
        REQUIRE ( 0.0 == TestScorer ( ds, normQuery ).QueryCoordination ( normField ) ); // no terms in the address
    }

    SECTION ("QueryNormalization")
    {
        FAIL("TODO");
    }

    SECTION ("Score, full")
    {
        FAIL("TODO");
    }

}
