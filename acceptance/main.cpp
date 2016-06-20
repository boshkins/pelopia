#include <iostream>

#define CATCH_CONFIG_RUNNER
#include <test/catch.hpp>

#include <pelopia/Dataset.h>
#include <pelopia/GeocodeJSON.h>

using namespace Mapzen::Pelopia;
using namespace std;

TEST_CASE("Story 2.1. one term, no location")
{
    Dataset ds("./input/1.json");

    SECTION ("found 10 or more")
    {
        const string Term = "Warwickshire";
        const size_t ExpectedCount = 10;

        const Response& resp = ds.Search ( Term . c_str () );

        REQUIRE ( DefaultResults == resp . Count () );

        for (unsigned int i = 0; i < resp.Count(); ++i)
        {
            Id id = resp.Get(i);
            const GeocodeJSON::Feature& place = ds.Place(id);
            REQUIRE ( string ( place . Label () ) . find ( Term ) != string :: npos );
        }
    }
    SECTION ( "found less than 10" )
    {
        const string Term = "Bermuda";

        const Response& resp = ds.Search ( Term . c_str () );

        REQUIRE ( 6 == resp . Count () );

        for (unsigned int i = 0; i < resp.Count(); ++i)
        {
            Id id = resp.Get(i);
            const GeocodeJSON::Feature& place = ds.Place(id);
            REQUIRE ( string ( place . Label () ) . find ( Term ) != string :: npos );
        }
    }

    SECTION ( "found none" )
    {
        const Response& resp = ds.Search ( "Salisbury" );
        REQUIRE ( 0 == resp . Count () );
    }
}

TEST_CASE("story 2.2. one term, focus LatLon, found")
{   // there are 6 objects on a small street Warwickshire Road in Warwick, Bermuda.
    // Focus on 2-mile radius with a center nearby, make sure all objects are reported
    // and none from farther out (PA, FL). Check sort order, scoring
    Dataset ds("./input/1.json");
    // the order in the dataset:
    //  "8 Warwickshire Road, Warwick, Bermuda" -64.810363, 32.260517
    //  "3 Warwickshire Road, Warwick, Bermuda" -64.810874, 32.260458
    //  "1 Warwickshire Road, Warwick, Bermuda" -64.810976, 32.260702
    //  "5 Warwickshire Road, Warwick, Bermuda" -64.810646, 32.260833
    //  "4 Warwickshire Road, Warwick, Bermuda" -64.810274, 32.260261
    //  "6 Warwickshire Road, Warwick, Bermuda" -64.810575, 32.260134

    const string Term = "Warwickshire";
    LatLon focus ( 32.255, -64.805 ); // this is ~0.5 miles to the SouthEast from the street

    SECTION ( "unbound, sort order" )
    {
        const Response& resp = ds.Search ( Term . c_str (), focus ); // default distance = 0 ( unbound ) , default results = 10
        REQUIRE ( DefaultResults == resp . Count () );
        REQUIRE ( DefaultResults >= 6 );
        // make sure top 6 are in Bermuda, sorted correctly (by distance from focus)
        Distance dist;
        {
            auto& f ( ds.Place( resp.Get( 0 ) ) );
            REQUIRE ( string ( "4 Warwickshire Road, Warwick, Bermuda" ) == f.Label() );   // -64.810274, 32.260261
            LatLon c ( f.GetGeometry()->Latitude(), f.GetGeometry()->Longitude() );
            Distance new_dist = focus.DistanceTo ( c );
        }
        {
            auto& f ( ds.Place( resp.Get( 1 ) ) );
            REQUIRE ( string ( "6 Warwickshire Road, Warwick, Bermuda" ) == f.Label() );   // -64.810575, 32.260134
            LatLon c ( f.GetGeometry()->Latitude(), f.GetGeometry()->Longitude() );
            Distance new_dist = focus.DistanceTo ( c );
            REQUIRE ( new_dist.GetKilometers() >= dist.GetKilometers() );
            dist = new_dist;
        }
        {
            auto& f ( ds.Place( resp.Get( 2 ) ) );
            REQUIRE ( string ( "8 Warwickshire Road, Warwick, Bermuda" ) == f.Label() );   // -64.810363, 32.260517
            LatLon c ( f.GetGeometry()->Latitude(), f.GetGeometry()->Longitude() );
            Distance new_dist = focus.DistanceTo ( c );
            REQUIRE ( new_dist.GetKilometers() >= dist.GetKilometers() );
            dist = new_dist;
        }
        {
            auto& f ( ds.Place( resp.Get( 3 ) ) );
            REQUIRE ( string ( "3 Warwickshire Road, Warwick, Bermuda" ) == f.Label() );   // -64.810874, 32.260458
            LatLon c ( f.GetGeometry()->Latitude(), f.GetGeometry()->Longitude() );
            Distance new_dist = focus.DistanceTo ( c );
            REQUIRE ( new_dist.GetKilometers() >= dist.GetKilometers() );
            dist = new_dist;
        }
        {
            auto& f ( ds.Place( resp.Get( 4 ) ) );
            REQUIRE ( string ( "5 Warwickshire Road, Warwick, Bermuda" ) == f.Label() );   // -64.810646, 32.260833
            LatLon c ( f.GetGeometry()->Latitude(), f.GetGeometry()->Longitude() );
            Distance new_dist = focus.DistanceTo ( c );
            REQUIRE ( new_dist.GetKilometers() >= dist.GetKilometers() );
            dist = new_dist;
        }
        {
            auto& f ( ds.Place( resp.Get( 5 ) ) );
            REQUIRE ( string ( "1 Warwickshire Road, Warwick, Bermuda" ) == f.Label() );   // -64.810976, 32.260702
            LatLon c ( f.GetGeometry()->Latitude(), f.GetGeometry()->Longitude() );
            Distance new_dist = focus.DistanceTo ( c );
            REQUIRE ( new_dist.GetKilometers() >= dist.GetKilometers() );
            dist = new_dist;
        }
    }
}

int
main( int argc, char* argv[] )
{
	return Catch::Session().run( argc, argv );
}

