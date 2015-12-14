#include "ResponseInternal.h"

#include <stdexcept>

using namespace Mapzen :: Pelopia;
using namespace std;

ResponseInternal :: ResponseInternal ( unsigned int p_reserve )
: Response ( p_reserve )
{
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
