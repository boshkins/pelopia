#include <iostream>

#include <mapzen/pelopia.h>

using namespace Mapzen :: Pelopia;
using namespace Mapzen :: GeoJSON;
using namespace std;

void main ()
{
	Dataset ds ( "NYC.pelopia" );
	
	Response resp = ds . Search ( "Mapzen", LatLon ( 40.7507545, -73.9939152 ) );

	for ( unsigned int i = 0 ; i < resp . Count (); ++ i )
    {
        Id id;
        MatchQuality score;
        resp . Get ( i, id, score );
        Feature place = ds . Place ( id );
        cout << ( i + 1 ) << " ( " << score << " ) " 
             << ": " <<  place . Name () << ", " << place . Address () 
             << endl;
    }
}

