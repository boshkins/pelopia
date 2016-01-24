#include <pelopia/GeocodeJSON.h>

#include <sstream>

using namespace Mapzen :: Pelopia :: GeocodeJSON;
using namespace std;

Feature :: ~Feature ()
{
}

const char* 
Feature :: SearchableProperty ( SearchablePropertyId p_id ) const throw ( invalid_argument )
{
    switch ( p_id )
    {
        case Property_Label:        return Label();
        case Property_Name:         return Name();
        case Property_HouseNumber:  return HouseNumber();
        case Property_Street:       return Street();
        case Property_Postcode:     return Postcode();
        case Property_City:         return City();
        case Property_District:     return District();
        case Property_County:       return County();
        case Property_Region:       return Region();
        case Property_RegionAbbr:   return RegionAbbr();
        case Property_Country:      return Country();
        case Property_CountryAbbr:  return CountryAbbr();
        case Property_Geohash:      return Geohash();
        default:  
        {
            ostringstream s;
            s << "Feature :: SearchableProperty(" << (unsigned int)p_id << ") Id is out of range [" << (unsigned int)Property_begin << " .. " << (unsigned int)Property_end << ")";
            throw invalid_argument ( s . str() );             
        }      
    }
}
