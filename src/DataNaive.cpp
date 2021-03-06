#include "DataNaive.h"

#include <fstream>
#include <sstream>

#include "GeocodeJSON_Reader_Rapid_DOM.h"

using namespace Mapzen :: Pelopia;
using namespace Mapzen :: Pelopia :: GeocodeJSON;
using namespace std;

DataNaive :: DataNaive ( const std :: string& filename )  throw ( invalid_argument, logic_error )
: m_reader ( nullptr )
{
    try
    {
        ifstream in ( filename );
        if ( ! in . good () )
        {
            throw invalid_argument ( string ( "File '" ) + filename + "' could not be opened" );
        }
        
        m_reader = new Reader_Rapid_DOM ( in );
        while (true)
        {
            Feature* f = m_reader->NextValue ();
            if (f == nullptr)
            {
                break;
            }
            m_features . push_back ( f );
        }
    }
    catch (...)
    {
        Clear();
        throw;
    }
}

void
DataNaive :: Clear ()
{
    for ( Features :: iterator it = m_features . begin(); it != m_features . end(); ++it )
    {
        delete *it;
    }
    delete m_reader;
}            

DataNaive :: ~DataNaive()
{
    Clear();
}

const GeocodeJSON::Feature& 
DataNaive :: Get ( Id p_id ) const throw ( invalid_argument )
{
    if ( p_id == 0 || p_id > m_features . size () )
    {
        ostringstream s;
        s << "DataNaive :: Get(" << p_id << ") Id is out of range [1 .. " << m_features . size () << "]";
        throw invalid_argument ( s . str() ); 
    }
    return *m_features [ p_id - 1 ]; // Id is 1-based!
}

size_t 
DataNaive :: Count () const
{
    return m_features . size ();
}
