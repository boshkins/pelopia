#include <pelopia/Dataset.h>

#include <stdexcept>
#include <map>

#include "GeocodeJSON_Reader_Rapid_DOM.h"
#include "DataNaive.h"
#include "TextIndexNaive.h"
#include "NormalizerNaive.h"
#include "ResponseInternal.h"

using namespace Mapzen :: Pelopia;
using namespace std;

///////////// Dataset::Impl

class Dataset::Impl : public DataNaive
{
public:
    Impl(const char* p_filename) throw ( std :: invalid_argument, std :: logic_error )
    :   DataNaive ( p_filename )
    {
        TextIndexNaiveWriter w;
        
        // traverse data, add all terms to writer
        size_t count = DataNaive::Count();
        for ( size_t i = 0 ; i < count; ++i )
        {   // iterate over all searchable properties in Get(i+1), split into terms, normalize, add to writer
            for ( GeocodeJSON :: Feature :: SearchablePropertyId property = GeocodeJSON :: Feature :: Property_begin; 
                  property < GeocodeJSON :: Feature :: Property_end; 
                  property = (GeocodeJSON :: Feature :: SearchablePropertyId)((unsigned int) property + 1) )
            {
                const char* value = DataNaive::Get( i + 1 ).SearchableProperty ( property );
                if ( value != nullptr )
                {
                    const Normalizer::Result& res = m_norm.Normalize ( value, strlen ( value ) );
                    for ( Normalizer::Result::const_iterator it = res.begin(); it != res.end(); ++it )
                    {
                        w.AddTermUse ( string ( value + it->startBytes, it->lengthBytes ), //TODO( pass char32_t* it->m_norm ) to avoid an extra conversion from UTF8
                                       Id (i + 1 ), 
                                       property, 
                                       0,   // TODO: handle indexed properrties
                                       it->startBytes );
                    }
                }
            }
        }
        m_reader = new TextIndexNaiveReader ( w.DetachIndex () );
    }
    ~Impl()
    {
        delete m_reader;
    }
    
    typedef map<Id, MatchQuality> HitMap;
    
    const Response& Search ( const char* p_text, Format p_format )
    {
        HitMap hits;
        
        const Normalizer::Result& res = m_norm.Normalize ( p_text, strlen ( p_text ) );
        for ( Normalizer::Result::const_iterator it = res.begin(); it != res.end(); ++it )
        {
            const TextIndexReader :: Node* node = m_reader->Locate ( string ( p_text + it->startBytes, it->lengthBytes ) ); //TODO( pass char32_t* it->m_norm ) to avoid an extra conversion from UTF8)
            if ( node != nullptr )
            {   // add all uses of the term to the result set (filter out duplicates)
                const TextIndexReader :: Payload& uses = node->GetPayload();
                size_t count = uses.Count();
                for ( size_t i = 0; i < count; ++i )
                {   
                    Id id = uses.GetTermUse( i ).ObjectId();
                    // for now, assign matching scores in the reverse sort order
                    MatchQuality score = double ( i ) / count; //TODO: use a Scorer
                    
                    // insert or update max score
                    HitMap::iterator hit = hits.find(id);
                    if ( hit == hits.end() )
                    {
                        hits [ id ] = score;
                    }
                    else
                    {
                        hit->second = max ( hit->second, score );
                    }
                }
            }

            // copy hits to the results container
            m_response.Clear();
            for ( HitMap::const_iterator it = hits.begin(); it != hits.end(); ++it )
            {
                m_response.AppendMatch ( it->first, it->second );
            }
            
            // sort on descending score and exclude extra results
            m_response.Sort();
            m_response.Truncate ( p_format.maxResults );
        }
        
        return m_response;
    }
    
    NormalizerNaive         m_norm;
    TextIndexNaiveReader*   m_reader;
    ResponseInternal        m_response;
};

///////////// Dataset

Dataset :: Dataset ( const char* filename ) throw ( std :: invalid_argument, std :: logic_error )
{
    m_impl = new Impl(filename);
}            

Dataset :: ~Dataset ()
{
    delete m_impl;
}            

const Response&  
Dataset :: Search ( const char* p_text, Format p_format )
{
    return m_impl -> Search ( p_text, p_format );
}            

//TODO: all the methods below
const Response&  
Dataset :: Search ( const char* text,
                    const LatLon& scope,
                    const Distance& radius,
                    Format format )
{
    throw logic_error ( "not implemented" );
}            

const Response&  
Dataset :: Search ( const char* text,
                    const BoundingBox& scope,
                    Format format )
{
    throw logic_error ( "not implemented" );
}            
    
const Response& 
Dataset :: Autocomplete ( const char* text, const LatLon& scope )
{
    throw logic_error ( "not implemented" );
}            

const Response&  
Dataset :: Autocomplete ( const char* text, const BoundingBox& scope )
{
    throw logic_error ( "not implemented" );
}            

const Response&  
Dataset :: Reverse ( const LatLon& point,
                     const Distance& radius,
                     Format format )
{
    throw logic_error ( "not implemented" );
}            
                   
const GeocodeJSON :: Feature& 
Dataset :: Place ( Id p_id ) const  throw ( std :: invalid_argument )
{
    return m_impl->Get(p_id);
}            

