#include <pelopia/Dataset.h>

#include <stdexcept>

#include "GeocodeJSON_Reader_Rapid_DOM.h"
#include "DataNaive.h"

using namespace Mapzen :: Pelopia;
using namespace std;

///////////// Dataset::Impl

class Dataset::Impl : public DataNaive
{
public:
    Impl(const char* p_filename)
    :   DataNaive ( p_filename )
    {
    }
    ~Impl()
    {
    }
    
    Response m_lastResponse;
};

///////////// Dataset

Dataset :: Dataset ( const char* filename )
{
    m_impl = new Impl(filename);
}            

Dataset :: ~Dataset ()
{
    delete m_impl;
}            

const Response&  
Dataset :: Search ( const char* text, Format format )
{
    return m_impl -> m_lastResponse;
}            

//TODO: all the methods below
const Response&  
Dataset :: Search ( const char* text,
                    const LatLon& scope,
                    const Distance& radius,
                    Format format )
{
    throw logic_error ( "not implemented" );
}            

const Response&  
Dataset :: Search ( const char* text,
                    const BoundingBox& scope,
                    Format format )
{
    throw logic_error ( "not implemented" );
}            
    
const Response& 
Dataset :: Autocomplete ( const char* text, const LatLon& scope )
{
    throw logic_error ( "not implemented" );
}            

const Response&  
Dataset :: Autocomplete ( const char* text, const BoundingBox& scope )
{
    throw logic_error ( "not implemented" );
}            

const Response&  
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

