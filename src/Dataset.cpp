#include <pelopia/Dataset.h>

#include <stdexcept>

using namespace Mapzen :: Pelopia;
using namespace std;

Dataset :: Dataset ( const char* filename )
{
    throw logic_error ( "not implemented" );
}            

Response 
Dataset :: Search ( const char* text, Format format )
{
    throw logic_error ( "not implemented" );
}            

Response 
Dataset :: Search ( const char* text,
                    const LatLon& scope,
                    const Distance& radius,
                    Format format )
{
    throw logic_error ( "not implemented" );
}            

Response 
Dataset :: Search ( const char* text,
                    const BoundingBox& scope,
                    Format format )
{
    throw logic_error ( "not implemented" );
}            
    
Response
Dataset :: Autocomplete ( const char* text, const LatLon& scope )
{
    throw logic_error ( "not implemented" );
}            

Response 
Dataset :: Autocomplete ( const char* text, const BoundingBox& scope )
{
    throw logic_error ( "not implemented" );
}            

Response 
Dataset :: Reverse ( const LatLon& point,
                     const Distance& radius,
                     Format format )
{
    throw logic_error ( "not implemented" );
}            
                   
const GeocodeJSON :: Feature& 
Dataset :: Place ( Id ) const  throw ( std :: logic_error )
{
    throw logic_error ( "not implemented" );
}            

