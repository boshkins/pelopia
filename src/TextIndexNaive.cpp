/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Implementation of TextIndexNaive, an STL-based index into terms used in a Dataset
*
*/

#include "TextIndexNaive.h"

#include <cassert>
#include <vector>
#include <map>
#include <locale>
#include <sstream>

using namespace std;
using namespace Mapzen::Pelopia;

//////////////////////////////////// TextIndexNaive

class Mapzen::Pelopia::TextIndexNaive
{   // UTF-8 compatible prefix tree 
public:
    class TermUse : public TextIndexReader :: TermUse
    {
    public:
        TermUse ( Id                                                p_object, 
                  GeocodeJSON :: Feature :: SearchablePropertyId    p_property, 
                  size_t                                            p_index,
                  size_t                                            p_offset,
                  size_t                                            p_length )
        :   m_object ( p_object ), 
            m_property ( p_property ), 
            m_index ( p_index ),
            m_offset ( p_offset ),
            m_length ( p_length )
        {
        }
        
        virtual ~TermUse() {}
        
        virtual Id ObjectId () const 
        {
            return m_object;
        }
        virtual GeocodeJSON :: Feature :: SearchablePropertyId PropertyId () const 
        {
            return m_property;
        }
        
        virtual void Position ( size_t& p_index, size_t& p_offset, size_t& p_length ) const 
        {
            p_index = m_index;
            p_offset = m_offset;
            p_length = m_length;
        }
        
        Id                                               m_object; 
        GeocodeJSON :: Feature :: SearchablePropertyId   m_property; 
        size_t                                           m_index;
        size_t                                           m_offset;
        size_t                                           m_length;
    };
    
    class Payload : public TextIndexReader :: Payload 
    {
    public:
        virtual ~Payload() {}
        
        virtual size_t Count () const 
        {  
            return m_body.size();
        }
        
        virtual const TermUse& GetTermUse ( size_t p_index ) const throw ( TextIndexReader :: index_out_of_bounds ) 
        {
            if ( p_index >= Count() )
            {
                ostringstream s;
                s << "TextIndexNaive :: Payload :: GetTermUse(" << p_index << ") index is out of range [0 .. " << ( Count () - 1 ) << "]";
                throw TextIndexReader :: index_out_of_bounds ( s . str() ); 
            }
            return m_body [ p_index ];
        }
        
        void Add ( Id                                               p_object, 
                   GeocodeJSON :: Feature :: SearchablePropertyId   p_property, 
                   size_t                                           p_index,
                   size_t                                           p_offset,
                   size_t                                           p_length )
        {
            //TODO: maintain sort order by increasing object Id, then increasing property, then by increasing index, then by increasing position 
            m_body.push_back ( TermUse ( p_object, p_property, p_index, p_offset, p_length ));
        }
        
        vector<TermUse> m_body;
    };

    class Node : public TextIndexReader :: Node
    { 
    public:
        virtual ~Node() {}
        
        virtual const Payload& GetPayload () const 
        { 
            return m_payload;
        }

        virtual size_t ChildrenCount () const
        {
            return m_children.size();
        }
        
        virtual const Node& GetChild ( size_t index ) const throw ( TextIndexReader :: index_out_of_bounds ) 
        {
            throw logic_error("TextIndexNaive::Node::GetChild not implemented");
        }        
        
        void Add ( const char32_t*                                  p_term, 
                   Id                                               p_object, 
                   GeocodeJSON :: Feature :: SearchablePropertyId   p_property, 
                   size_t                                           p_index,
                   size_t                                           p_offset,
                   size_t                                           p_length )
        {
            if ( *p_term == 0 )
            {   // term complete; add to the payload
                m_payload.Add( p_object, p_property, p_index, p_offset, p_length );
            }
            else
            {   // continue down the tree
                Node& next = m_children [ *p_term ]; // will insert if does not exist
                next.Add ( p_term + 1, p_object, p_property, p_index, p_offset, p_length );
            }
        }
        
        const Node* Find ( const char32_t* p_term ) const
        {
            if ( *p_term == 0 )
            {
                return this;
            }
            Children::const_iterator next = m_children.find ( *p_term );
            if ( next == m_children.end() )
            {
                return nullptr;
            }
            return next->second.Find ( p_term + 1 );
        }
        
        typedef map<char32_t, Node> Children;
        
        Children    m_children;
        Payload     m_payload;
    }; 
    
private:
    template <class internT, class externT, class stateT> 
        struct codecvt : std::codecvt<internT,externT,stateT> { ~codecvt(){} };
        
    static u32string utf8toChar32 ( const string& p_term )
    {   // convert into a string of char32_t
        wstring_convert < codecvt < char32_t, char, mbstate_t >, char32_t > fromStringToString32;
        return fromStringToString32.from_bytes ( p_term.c_str() );
    }

public:    
    void Add ( const string&                                    p_term, 
               Id                                               p_object, 
               GeocodeJSON :: Feature :: SearchablePropertyId   p_property, 
              size_t                                            p_index,
               size_t                                           p_offset )
    {
        u32string term32 = utf8toChar32( p_term );
        m_root.Add ( term32.c_str(), p_object, p_property, p_index, p_offset, term32.size() );
    }
    
    const Node* Locate ( const string& p_term ) const
    {
        u32string term32 = utf8toChar32( p_term );
        return m_root.Find ( term32.c_str() );
    }      
    
public:    
    Node m_root;
};

//////////////////////////////////// TextIndexNaiveWriter
TextIndexNaiveWriter :: TextIndexNaiveWriter()
: m_index ( new TextIndexNaive )
{
}

TextIndexNaiveWriter :: ~TextIndexNaiveWriter()
{
    delete m_index;
}

void
TextIndexNaiveWriter :: AddTermUse ( const std :: string&                           p_term, 
                                     Id                                             p_object, 
                                     GeocodeJSON :: Feature :: SearchablePropertyId p_property, 
                                     size_t                                         p_index,
                                     size_t                                         p_offset )
{
    assert ( m_index );
    
    m_index -> Add ( p_term, p_object, p_property, p_index, p_offset );
}                           

TextIndexNaive* 
TextIndexNaiveWriter :: DetachIndex () // ownership transferred to the caller
{
    assert ( m_index );
    
    TextIndexNaive* ret = m_index;
    m_index = nullptr;
    return ret;
}
          
//////////////////////////////////// TextIndexNaiveReader
       
TextIndexNaiveReader :: TextIndexNaiveReader ( TextIndexNaive* p_index ) // assumes ownership of the index
: m_index ( p_index )
{
}

TextIndexNaiveReader :: ~TextIndexNaiveReader()
{
    delete m_index;
}
            
const TextIndexReader :: Node* 
TextIndexNaiveReader :: Locate ( const std :: string& p_term ) const
{
    assert ( m_index );
    
    return m_index->Locate ( p_term );
}