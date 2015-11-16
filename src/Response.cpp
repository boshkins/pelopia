#include <pelopia/Response.h>

#include <stdexcept>

using namespace Mapzen :: Pelopia;
using namespace std;

unsigned int 
Response :: Count() const
{
    throw logic_error ( "not implemented" );
}            

bool 
Response :: Get(unsigned int index, Id& id, MatchQuality& score) const
{
    throw logic_error ( "not implemented" );
}            

Id 
Response :: Get(unsigned int index) const
{
    throw logic_error ( "not implemented" );
}            

string 
Response :: Autocomplete(unsigned int index) const
{
    throw logic_error ( "not implemented" );
}            

