#include "GeocodeJSON_Reader_Rapid_DOM.h"

#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/encodings.h"
#include "rapidjson/error/en.h"

using namespace Mapzen :: Pelopia :: GeocodeJSON;
using namespace std;
using namespace rapidjson;

class Reader_Rapid_DOM :: StlStream // implements concept rapidjson :: Stream using std :: istream
{
public:    
    typedef std :: istream :: char_type Ch;    //!< Character type of the stream.
    
public:
    StlStream ( std :: istream& p_in )
    : m_in ( p_in )
    {
    }
    
    //! Read the current character from stream without moving the read cursor.
    Ch Peek() const 
    { 
        Ch ret = m_in . peek ();
        if ( ret == char_traits<Ch>::eof() )
        {
            ret = 0;
        }
        return ret; 
    }

    //! Read the current character from stream and moving the read cursor to next character.
    Ch Take() 
    { 
        Ch ret = m_in . get ();
        if ( ret == char_traits<Ch>::eof() )
        {
            ret = 0;
        }
        return ret; 
    }

    //! Get the current read cursor.
    //! \return Number of characters read from start.
    size_t Tell() 
    { 
        streampos ret = m_in . tellg ();
        if ( ret == -1 )
        {
            ret = 0;
        }
        cout << "StlStream::Tell(): " << ret << endl;
        return size_t ( ret ); 
    }

public:
    // not implemented for read-only streams
    Ch* PutBegin()      { throw std :: logic_error ( "StlStream::PutBegin() is not implemented" ); }
    void Put(Ch)        { throw std :: logic_error ( "StlStream::Put() is not implemented" ); }
    void Flush()        { throw std :: logic_error ( "StlStream::Flush() is not implemented" ); }
    size_t PutEnd(Ch*)  { throw std :: logic_error ( "StlStream::PutEnd() is not implemented" ); }
    
private:
    std :: istream& m_in;
};

Reader_Rapid_DOM :: Reader_Rapid_DOM ( std :: istream& p_in ) throw ( std :: logic_error )
{
    StlStream in ( p_in );
    m_doc . ParseStream < StlStream > ( in );
    if ( m_doc . HasParseError() )
    {
        ostringstream s;
        s << "JSON parse error at offset " << m_doc . GetErrorOffset () << " : " << GetParseError_En ( m_doc . GetParseError() );
        throw std :: logic_error ( s . str() ); 
    }
}
        
Reader_Rapid_DOM :: ~Reader_Rapid_DOM ()
{
}
        
const Feature* 
Reader_Rapid_DOM :: NextValue () throw ( std :: logic_error )
{
    return 0;
}

