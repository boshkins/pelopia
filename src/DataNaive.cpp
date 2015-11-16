#include "DataNaive.h"

#include <fstream>

using namespace Mapzen :: Pelopia;
using namespace std;

DataNaive :: DataNaive ( const std :: string& filename )  throw ( invalid_argument )
{
    ifstream in ( filename );
    if ( ! in . good () )
    {
        throw invalid_argument ( string ( "File '" ) + filename + "' could not be opened" );
    }
}            

DataNaive :: ~DataNaive()
{
}

const GeocodeJSON::Feature& 
DataNaive :: Get ( Id ) const
{
    throw logic_error ( "not implemented" );
}

size_t 
DataNaive :: Count () const
{
    throw logic_error ( "not implemented" );
}
