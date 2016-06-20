#include "LocationFilter.h"

#include <sstream>

using namespace Mapzen :: Pelopia;
using namespace std;

LocationFilter :: LocationFilter ( Id p_maxId )
: m_body ( p_maxId + 1 )
{
}

Id
LocationFilter :: MaxId () const
{
    return m_body.size() - 1;
}

void
LocationFilter :: CheckId ( Id p_id ) const
{
    if ( p_id == InvalidId || p_id > MaxId() )
    {
        ostringstream s;
        s << "LocationFilter :: IsSet(" << p_id << ") Id is out of range [1 .. " << MaxId () << "]";
        throw out_of_range ( s.str() );
    }
}

void
LocationFilter :: Set ( Id p_id )
{
    CheckId( p_id );
    m_body [ p_id ] = true;
}

void
LocationFilter :: Unset ( Id p_id )
{
    CheckId( p_id );
    m_body [ p_id ] = false;
}

bool
LocationFilter :: IsSet ( Id p_id ) const
{
    CheckId( p_id );
    return m_body [ p_id ];
}

LocationFilter&
LocationFilter ::  And ( const LocationFilter& p_rhs )
{
    if ( MaxId() != p_rhs . MaxId() )
    {
        ostringstream s;
        s << "LocationFilter :: And() filter sizes differ (" << MaxId () << " and " << p_rhs . MaxId () << ")";
        throw invalid_argument ( s.str() );
    }
    Id count = min ( MaxId(), p_rhs . MaxId () );
    for ( Id i = 0; i < count; ++i )
    {
        Id id = i+1;
        if ( IsSet ( id ) && ! p_rhs . IsSet ( id ) )
        {
            Unset ( id );
        }
    }
    return *this;
}