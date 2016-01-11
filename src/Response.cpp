#include <pelopia/Response.h>

#include <stdexcept>

using namespace Mapzen :: Pelopia;
using namespace std;

Response :: Response ( unsigned int p_reserve )
{
    m_matches.reserve ( p_reserve );
}

unsigned int 
Response :: Count() const
{
    return m_matches.size();
}            

bool 
Response :: Get(unsigned int p_index, Id& p_id, MatchQuality& p_score) const 
{
    if ( p_index >= Count() )
    {
        return false;
    }
    const Match& m = m_matches[p_index];
    p_id = m.id;
    p_score = m.score;
    return true;
}            

Id 
Response :: Get(unsigned int p_index) const
{
    if ( p_index >= Count() )
    {
        return InvalidId;
    }
    return m_matches[p_index].id;
}            

string 
Response :: Autocomplete(unsigned int index) const
{
    throw logic_error ( "not implemented" );
}            

