#include <iostream>

#include <pelopia/Dataset.h>
#include <pelopia/GeoJSON.h>

using namespace Mapzen::Pelopia;
using namespace std;

void main()
{
	Dataset ds("NYC.pelopia");

	Response resp = ds.Search("Mapzen", LatLon(40.7507545, -73.9939152));

	for (unsigned int i = 0; i < resp.Count(); ++i)
	{
		Id id;
		MatchQuality score;
		resp.Get(i, id, score);
		GeoJSON::Feature place = ds.Place(id);
		cout << (i + 1) << " ( " << score << " ) "
			<< ": " << place.Name() << ", " << place.Address()
			<< endl;
	}
}
