#include <pelopia/GeocodeJSON.h>

#include <stdexcept>

using namespace Mapzen :: Pelopia;
using namespace Mapzen :: Pelopia :: GeocodeJSON;
using namespace std;

Coordinate 
Geometry :: Latitude() const
{
    throw logic_error ( "not implemented" );
}            

Coordinate 
Geometry :: Longitude() const
{
    throw logic_error ( "not implemented" );
}            

const Geometry* 
Feature :: GetGeometry() const
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Id () const
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Layer () const
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Source () const // optional
{
    throw logic_error ( "not implemented" );
}            

uint32_t 
Feature :: AccuracyMeters () const // optional
{
    throw logic_error ( "not implemented" );
}            

double 
Feature :: Confidence () const // 0.0 .. 1.0
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Label () const
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Name () const // "recommended" but still optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: HouseNumber () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Street () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Postcode () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: City () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: District () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: County () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Region () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: RegionAbbr () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Country () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: CountryAbbr () const // optional
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Admin ( unsigned int level ) const // optional; level from 1 to 10
{
    throw logic_error ( "not implemented" );
}            

const char* 
Feature :: Geohash () const // optional
{
    throw logic_error ( "not implemented" );
}            

const Geometry& 
Feature :: CenterPoint() const
{
    throw logic_error ( "not implemented" );
}            

std::string 
Feature :: Stringify() const
{
    throw logic_error ( "not implemented" );
}            
