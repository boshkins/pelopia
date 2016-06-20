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
    NormalizerNaive norm;
    Normalizer::Result normQuery;

    class TestScorer : public TextScorer
    {
    public:
        TestScorer ( const Dataset & p_ds, const Normalizer& p_norm, const Normalizer :: Result & p_query )
        : TextScorer ( p_ds, p_norm, p_query ) {}

        using TextScorer::TermFrequency;
        using TextScorer::InverseDocumentFrequency;
        using TextScorer::TermWeight;
        using TextScorer::QueryCoordination;
        using TextScorer::QueryNormalization;
    };

    {
        Normalizer::Result normField;
        norm.Normalize ( ds.Place(1).Label(), normField );

        // TF = sqrt ( # of apperances in the address ) = sqrt(1)
        SECTION ("TermFrequency, 1")
        {
            norm.Normalize ("Warwickshire", normQuery );
            REQUIRE ( 1 == TestScorer ( ds, norm, normQuery ).TermFrequency ( normField, normQuery.m_terms [ 0 ].norm ) );
        }
        //TODO: add a section that produces something other than 0 or 1
        SECTION ("TermFrequency, 0")
        {
            norm.Normalize ( "Sussex", normQuery );
            REQUIRE ( 0 == TestScorer ( ds, norm, normQuery ).TermFrequency ( normField, normQuery.m_terms [ 0 ].norm ) );
        }
    }

    // IDF = 1 + log ( totalFeatures / featuresContainingTheTerm ) = 1 + log ( 10 / 10 ) = 1
    SECTION ("InverseDocumentFrequency, very popular")
    {
        norm.Normalize ("Warwickshire", normQuery );
        double idf = TestScorer ( ds, norm, normQuery ).InverseDocumentFrequency ( normQuery.m_terms [ 0 ].norm );
        REQUIRE ( 0.9046 == Approx ( idf ).epsilon ( 0.0001 ) );
    }
    SECTION ("InverseDocumentFrequency, less popular")
    {
        norm.Normalize ( "Bermuda", normQuery );
        double idf = TestScorer ( ds, norm, normQuery ).InverseDocumentFrequency ( normQuery.m_terms [ 0 ].norm );
        REQUIRE ( 1.3566 == Approx ( idf ).epsilon ( 0.0001 ) );
    }
    SECTION ("InverseDocumentFrequency, absent")
    {
        norm.Normalize ( "Sussex", normQuery );
        double idf = TestScorer ( ds, norm, normQuery ).InverseDocumentFrequency ( normQuery.m_terms [ 0 ].norm );
        REQUIRE ( 3.3025 == Approx ( idf ).epsilon ( 0.0001 ) );
    }

    // TermWeight = TF * IDF * ( 1 / sqrt # of terms in the address )
    SECTION ("TermWeight Id 1")
    {
        Normalizer::Result normField;
        norm.Normalize ( ds.Place(1).Label(), normField ); // 3437 Warwickshire Road, Buckingham, PA
        norm.Normalize ( "Warwickshire", normQuery );
        double tw = TestScorer ( ds, norm, normQuery ).TermWeight ( normField, normQuery.m_terms [ 0 ].norm );
        REQUIRE ( 0.4045 == Approx ( tw ).epsilon ( 0.0001 ) );
    }
    SECTION ("TermWeight Id 2")
    {
        Normalizer::Result normField;
        norm.Normalize ( ds.Place(2).Label(), normField );  // 8 Warwickshire Road, Warwick, Bermuda
        norm.Normalize ( "Bermuda", normQuery );
        double tw = TestScorer ( ds, norm, normQuery ).TermWeight ( normField, normQuery.m_terms [ 0 ].norm );
        REQUIRE ( 0.6067 == Approx ( tw ).epsilon ( 0.0001 ) ); // "Bermuda" is less frequent in the dataset than "Warwickshire", thus the score is higher than in the previous section
    }
    SECTION ("TermWeight, not found")
    {
        Normalizer::Result normField;
        norm.Normalize ( ds.Place(2).Label(), normField );  // 8 Warwickshire Road, Warwick, Bermuda
        norm.Normalize ( "Solovki", normQuery );
        double tw = TestScorer ( ds, norm, normQuery ).TermWeight ( normField, normQuery.m_terms [ 0 ].norm );
        REQUIRE ( 0.0 == tw ); // not in the address
    }

    SECTION ("QueryCoordination, all terms")
    {
        Normalizer::Result normField;
        norm.Normalize ( ds.Place(2).Label(), normField );  // 8 Warwickshire Road, Warwick, Bermuda
        norm.Normalize ( "Warwickshire Road Warwick Warwick", normQuery );
        REQUIRE ( 1 == TestScorer ( ds, norm, normQuery ).QueryCoordination ( normField ) ); // all terms are in the address
    }
    SECTION ("QueryCoordination, some terms")
    {
        Normalizer::Result normField;
        norm.Normalize ( ds.Place(2).Label(), normField );  // 8 Warwickshire Road, Warwick, Bermuda
        norm.Normalize ( "Big House Warwick Bermuda", normQuery );
        REQUIRE ( 0.5 == TestScorer ( ds, norm, normQuery ).QueryCoordination ( normField ) ); // 2 out of 4 terms are in the address
    }
    SECTION ("QueryCoordination, not in address")
    {
        Normalizer::Result normField;
        norm.Normalize ( ds.Place(2).Label(), normField );  // 8 Warwickshire Road, Warwick, Bermuda
        norm.Normalize ( "Solovki building 1", normQuery );
        REQUIRE ( 0.0 == TestScorer ( ds, norm, normQuery ).QueryCoordination ( normField ) ); // no terms in the address
    }

    SECTION ("QueryNormalization")
    {
        norm.Normalize ( "Warwickshire Bermuda", normQuery );
        // IDF("Warwickshire") == 0.9046
        // IDF("Bermuda") == 1.3566
        // QNORM == 1 / sqrt (0.9046 + 1.3566) == 1 / 1.5037 == 0.6650
        REQUIRE ( 0.6650 == Approx ( TestScorer ( ds, norm, normQuery ).QueryNormalization() ).epsilon ( 0.0001 ) );
    }

    SECTION ("Score, full")
    {   // Score = QueryNormalizationFactor * QueryCoordination * sum ( TermWeight, for every term )
        norm.Normalize ( "Bermuda Triangle", normQuery );
        TestScorer ts ( ds, norm, normQuery );

        // address ( 2 ) : 8 Warwickshire Road, Warwick, Bermuda

        // QNORM == 1 / sqrt (1.3566 + 3.3025) == 0.4632
        REQUIRE ( 1.3566 == Approx ( ts.InverseDocumentFrequency ( normQuery.m_terms [ 0 ].norm ) ).epsilon ( 0.0001 ) );
        REQUIRE ( 3.3025 == Approx ( ts.InverseDocumentFrequency ( normQuery.m_terms [ 1 ].norm ) ).epsilon ( 0.0001 ) );
        REQUIRE ( 0.4632 == Approx ( ts.QueryNormalization() ).epsilon ( 0.0001 ) );

        // QCOORD == 0.5
        // TermWeight ("Bermuda") == 0.6067
        // TermWeight ("Triangle") == 0
        // SCORE == 0.4632 * 0.5 * ( 0.6067 + 0 ) == 0.1405
        REQUIRE ( 0.1405 == Approx ( ts.Score( 2 ) ).epsilon ( 0.0001 ) );
    }

}
