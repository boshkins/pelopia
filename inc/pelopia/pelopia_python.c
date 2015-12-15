
#include <pelopia/Dataset.h>
#include <pelopia/Response.h>
#include <pelopia/GeoJSON.h>

using namespace Mapzen::Pelopia;
using namespace std;

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
	
	return (void*)&(ds_c -> Search(text, LatLon(lat,lon), Distance(Miles, radius), Format(format)));
}

bool Get_py(void *resp, unsigned int index, unsigned int *id, double *score)
{
	
	Response *resp_c = (Response*)resp;
    	
   	return  resp_c -> Get(i, *id, *score); 
}

void *Place_py(void *ds, unsigned int *id)
{
	Dataset *ds_c = (Dataset*)ds;
    
    return (void*)&(ds_c -> Place(id));
}
