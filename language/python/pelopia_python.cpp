
#include <pelopia/Dataset.h>
#include <pelopia/Response.h>
#include <pelopia/GeocodeJSON.h>

using namespace Mapzen::Pelopia;
using namespace std;

extern "C" {
    
void *createDataset(const char *filename)
{
     Dataset *ds = new Dataset(filename);
     return (void *)ds;
}

void *Search_py(void *ds, 
	const char *text, 
	const double lat, 
	const double lon, 
	const double radius, 
	int format)
{
    Dataset *ds_c = (Dataset*)ds;
	Format fmt = { (unsigned int)format };
    
	return (void*)&(ds_c -> Search(text, fmt)); 
//	return (void*)&(ds_c -> Search(text, LatLon(lat,lon), Distance(Distance::Miles, radius), fmt));
}

bool Get_py(void *resp, unsigned int index, unsigned int *id, double *score)
{
	
	Response *resp_c = (Response*)resp;
    Id out_id;
    MatchQuality out_score;
   	if ( resp_c -> Get(index, out_id, out_score) )
    {
        *id = out_id;
        *score = out_score;
        return true;
    }
    else
    {
        return false;
    } 
}

void *Place_py(void *ds, unsigned int *id)
{
	Dataset *ds_c = (Dataset*)ds;
    
    return (void*)&(ds_c -> Place(*id));
}

void Cout_py(unsigned int index, double *score, void *place_id)
{
    Place *place_c = (Place*)place_id;
    double *score_c = (double*)score;

    cout << ( i + 1 ) << " ( " << score_c << " ) " 
             << ": " <<  place_c -> Name () << ", " << place_c -> Address () 
             << endl;
}             

}

