#include "ResponseInternal.h"

#include <stdexcept>
#include <algorithm>

using namespace Mapzen :: Pelopia;
using namespace std;

ResponseInternal :: ResponseInternal ( unsigned int p_reserve )
: Response ( p_reserve )
{
}

void
ResponseInternal :: Clear ()
{
    m_matches.clear();
}

void
ResponseInternal :: AppendMatch ( Id id, MatchQuality score )
{
    Match m = { id, score };
    m_matches.push_back ( m );
}

void
ResponseInternal :: AppendAutocomplete ( const std::string& )
{
    throw logic_error ( "not implemented" );
}


bool 
ResponseInternal :: Greater ( const Response::Match& p_a, const Response::Match& p_b )
{
    return p_a.score > p_b.score;
}  
    
void
ResponseInternal :: Sort ()
{
    sort ( m_matches.begin(), m_matches.end(), Greater );
}

void
ResponseInternal :: Truncate ( size_t p_members )
{
    if ( m_matches.size() > p_members )
    {
        m_matches.erase ( m_matches.begin() + p_members, m_matches.end() );
    }
}
    
