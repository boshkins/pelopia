#include "GeocodeJSON_Reader_Rapid_DOM.h"

#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/encodings.h"
#include "rapidjson/error/en.h"

using namespace Mapzen :: Pelopia :: GeocodeJSON;
using namespace std;
using namespace rapidjson;

class Reader_Rapid_DOM :: FeatureDOM : public Feature
{
public:    
    FeatureDOM ( const Value& p_value, Document::AllocatorType& p_allocator )
     :  m_allocator ( p_allocator ),
        m_value ( p_value, p_allocator )
    {
    }
    virtual ~FeatureDOM () 
    {
    }

public:    
    virtual const BoundingBox* GetBoundingBox() const 
    { 
        bool found;
        const Value& bb = GetOptionalMember ( m_value, "bbox", found );
        if ( ! found )
        {
            return nullptr; 
        }
        if ( ! bb . IsArray () || bb . Size () != 4 )
        {
            throw std :: logic_error ( "Property \"bbox\" is not an array of 4" ); 
        }
        
        // BBox uses Top-Left-Bottom-Right
        // GeocodeJSON goes Left-Bottom-Right-Top
        m_bbox . SetCoordinates ( bb [ 3 ] . GetDouble (), bb [ 0 ] . GetDouble (), bb [ 1 ] . GetDouble (), bb [ 2 ] . GetDouble () );
        return & m_bbox; 
    }

    virtual const Geometry* GetGeometry() const 
    { 
        const Value& g = GetMember ( "geometry" );
        if ( ! g . IsObject () )
        {
            throw std :: logic_error ( "Property \"geometry\" is not an object" ); 
        }
        const Value& coords = GetMember ( g, "coordinates" );
        if ( ! coords . IsArray () || coords . Size () != 2 )
        {
            throw std :: logic_error ( "Property \"coordinates\" is not an array of 2" ); 
        }
        
        // GeocodeJSON soecifies geometry as Lon-Lat
        m_geometry . SetLongitude ( coords [ 0 ] . GetDouble () );
        m_geometry . SetLatitude ( coords [ 1 ] . GetDouble ());
        
        return & m_geometry; 
    } 

    virtual const char* Id () const     { return GetStringProperty ( "id", true ); }
    virtual const char* Layer () const  { return GetStringProperty ( "layer", true ); }
    virtual const char* Source () const { return GetStringProperty ( "source" ); }
     
    virtual uint32_t AccuracyMeters () const 
    { 
        const Value& g = GetMember ( "properties" );
        if ( ! g . IsObject () )
        {
            throw std :: logic_error ( "Property \"properties\" is not an object" ); 
        }
        bool found;
        const Value& v = GetOptionalMember ( g, "accuracy", found );
        if ( ! found )
        {
            return 0; 
        }
        if ( ! v . IsUint () )
        {
            throw std :: logic_error ( string ( "Property \"" ) + "accuracy" +"\" is not an unsigned integer" ); 
        }
        return v . GetUint (); 
    }
    
    virtual double Confidence () const 
    { 
        const Value& g = GetMember ( "properties" );
        if ( ! g . IsObject () )
        {
            throw std :: logic_error ( "Property \"properties\" is not an object" ); 
        }
        bool found;
        const Value& v = GetOptionalMember ( g, "confidence", found );
        if ( ! found )
        {
            return 0; 
        }
        if ( ! v . IsDouble () )
        {
            throw std :: logic_error ( string ( "Property \"" ) + "confidence" +"\" is not a number" ); 
        }
        return v . GetDouble (); 
    }
    
    virtual const char* Label () const  { return GetStringProperty ( "label" ); }
    virtual const char* Name () const   { return GetStringProperty ( "name" ); }
    
    virtual const char* HouseNumber () const    { return GetStringProperty ( "housenumber" ); }
    virtual const char* Street () const         { return GetStringProperty ( "street" ); }
    virtual const char* Postcode () const       { return GetStringProperty ( "postcode" ); }
    virtual const char* City () const           { return GetStringProperty ( "city" ); }
    virtual const char* District () const       { return GetStringProperty ( "district" ); }
    virtual const char* County () const         { return GetStringProperty ( "county" ); }
    virtual const char* Region () const         { return GetStringProperty ( "region" ); }
    virtual const char* RegionAbbr () const     { return GetStringProperty ( "region_abbr", true ); }
    virtual const char* Country () const        { return GetStringProperty ( "country" ); }
    virtual const char* CountryAbbr () const    { return GetStringProperty ( "country_abbr" ); }
    
    virtual const char* Admin ( unsigned int p_level ) const 
    { 
        const Value& g = GetMember ( "properties" );
        if ( ! g . IsObject () )
        {
            throw std :: logic_error ( "Property \"properties\" is not an object" ); 
        }
        bool found;
        const Value& v = GetOptionalMember ( g, "admin", found );
        if ( ! found )
        {
            return nullptr; 
        }
        
        switch ( p_level )
        {
            case 1:     return GetStringProperty ( v, "level1" );
            case 2:     return GetStringProperty ( v, "level2" );
            case 3:     return GetStringProperty ( v, "level3" );
            case 4:     return GetStringProperty ( v, "level4" );
            case 5:     return GetStringProperty ( v, "level5" );
            case 6:     return GetStringProperty ( v, "level6" );
            case 7:     return GetStringProperty ( v, "level7" );
            case 8:     return GetStringProperty ( v, "level8" );
            case 9:     return GetStringProperty ( v, "level9" );
            case 10:    return GetStringProperty ( v, "level10" );
            default:    return nullptr;
        }
    } 
    
    virtual const char* Geohash () const { return GetStringProperty ( "geohash" ); }
    
    virtual std::string Stringify() const { return string(); }
    
    // create a deep copy of this object, to be deleted by the caller
    virtual Feature* Clone() const 
    {
         return new FeatureDOM ( m_value, m_allocator );
    } 
    
private:
    const Value& GetMember ( const Value& p_val, const char* p_name ) const throw ( std :: logic_error )
    {
        Value :: ConstMemberIterator it = p_val . FindMember ( p_name );
        if ( it == p_val . MemberEnd() )
        {
            throw std :: logic_error ( string ( "Property " ) + p_name + " does not exist" );
        }
        return it -> value;
    }
    
    const Value& GetOptionalMember ( const Value& p_val, const char* p_name, bool & p_found ) const
    {
        Value :: ConstMemberIterator it = p_val . FindMember ( p_name );
        p_found = ( it != p_val . MemberEnd() );
        return it -> value;
    }
    
    const Value& GetMember ( const char* p_name ) const throw ( std :: logic_error )
    {
        return GetMember ( m_value, p_name );
    }
    
    const char* GetStringProperty ( const Value& p_val, const char* p_name, bool p_mandatory = false ) const throw ( std :: logic_error )
    {
        if ( ! p_val . IsObject () )
        {
            throw std :: logic_error ( "Property \"properties\" is not an object" ); 
        }
        if ( p_mandatory )
        {
            const Value& v = GetMember ( p_val, p_name );
            if ( ! v . IsString () )
            {
                throw std :: logic_error ( string ( "Property \"" ) + p_name +"\" is not a character string" ); 
            }
            return v . GetString (); 
        }
        else
        {   // if absent, do not throw, return nullptr
            Value :: ConstMemberIterator it = p_val . FindMember ( p_name );
            if ( it == p_val . MemberEnd() )
            {
                return nullptr;
            }
            if ( ! it -> value . IsString () )
            {
                throw std :: logic_error ( string ( "Property \"" ) + p_name +"\" is not a character string" ); 
            }
            return it -> value . GetString ();
        }
    }
    
    const char* GetStringProperty ( const char* p_name, bool p_mandatory = false ) const throw ( std :: logic_error )
    {
        return GetStringProperty ( GetMember ( "properties" ), p_name );
    }
    
private:
    Document::AllocatorType&    m_allocator;
    Value                       m_value;
    mutable Geometry            m_geometry;
    mutable BoundingBox         m_bbox;
};

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
        if ( ret == char_traits<Ch>::eof() )
        {
            ret = 0;
        }
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
    m_features = m_doc [ "features" ];
    if ( ! m_features . IsArray () )
    {
        throw std :: logic_error ( "Property \"features\" is not an array" ); 
    }
    m_curFeature = 0;
}
        
Reader_Rapid_DOM :: ~Reader_Rapid_DOM ()
{
}
        
Feature* 
Reader_Rapid_DOM :: NextValue () throw ( std :: logic_error )
{
    if ( m_curFeature >= m_features . Size() )
    {
        return nullptr;
    }       
    Feature* ret = new FeatureDOM ( m_features [ m_curFeature ], m_doc.GetAllocator() );
    ++m_curFeature;
    return ret;
}

