#include <pelopia/Dataset.h>

#include <stdexcept>
#include <map>
#include <limits>

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
    :   DataNaive ( p_filename ),
        m_maxDistance ( Distance::Kilometers, 0 )
    {
        TextIndexNaiveWriter w;

        // traverse data, add all terms to writer, calculate max distance
        Coordinate minLat   = numeric_limits<Coordinate>::max();
        Coordinate minLon   = numeric_limits<Coordinate>::max();
        Coordinate maxLat   = numeric_limits<Coordinate>::lowest();
        Coordinate maxLon   = numeric_limits<Coordinate>::lowest();

        size_t count = DataNaive::Count();
        for ( size_t i = 0 ; i < count; ++i )
        {   // iterate over all searchable properties in Get(i+1), split into terms, normalize, add to writer
            // calculate the maximum distance between features ion the dataset ( minLat,minLon to maxLat,maxLon )
            const GeocodeJSON :: Feature& feature = DataNaive::Get( i + 1 );

            for ( GeocodeJSON :: Feature :: SearchablePropertyId property = GeocodeJSON :: Feature :: Property_begin;
                  property < GeocodeJSON :: Feature :: Property_end;
                  property = (GeocodeJSON :: Feature :: SearchablePropertyId)((unsigned int) property + 1) )
            {
                const char* value = feature.SearchableProperty ( property );
                if ( value != nullptr )
                {
                    const Normalizer::Result& res = m_norm.Normalize ( value );
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

            // update min/max coords
            minLat = min ( minLat, feature.GetGeometry()->Latitude() );
            minLon = min ( minLon, feature.GetGeometry()->Longitude() );
            maxLat = max ( maxLat, feature.GetGeometry()->Latitude() );
            maxLon = max ( maxLon, feature.GetGeometry()->Longitude() );
        }

        m_reader = new TextIndexNaiveReader ( w.DetachIndex () );
        m_maxDistance = LatLon ( minLat, minLon ).DistanceTo ( LatLon ( maxLat, maxLon ) );
    }
    ~Impl()
    {
        delete m_reader;
    }

    typedef map<Id, MatchQuality> HitMap;

    const Response& Search ( const char* p_text, Format p_format )
    {   // no geofiltering
        HitMap hits;

        const Normalizer::Result& res = m_norm.Normalize ( p_text );
        for ( Normalizer::Result::const_iterator it = res.begin(); it != res.end(); ++it )
        {
            const TextIndexReader :: Node* node = m_reader->Locate ( it->norm );
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

    const Response& Search ( const char*        p_text,
                             const LatLon&      p_scope,
                             const Distance&    p_radius,
                             Format             p_format )
    {
        return Search ( p_text, p_format );
    }

    NormalizerNaive         m_norm;
    TextIndexNaiveReader*   m_reader;
    ResponseInternal        m_response;
    Distance                m_maxDistance;
};

///////////// Dataset

Dataset::Dataset ( const char* filename ) throw ( std :: invalid_argument, std :: logic_error )
{
    m_impl = new Impl(filename);
}

Dataset::~Dataset ()
{
    delete m_impl;
}

const Response&
Dataset::Search ( const char* p_text, Format p_format )
{
    return m_impl -> Search ( p_text, p_format );
}

const Response&
Dataset::Search ( const char*     p_text,
                    const LatLon&   p_scope,
                    const Distance& p_radius,
                    Format          p_format )
{
    return m_impl -> Search ( p_text, p_scope, p_radius, p_format );
}

const GeocodeJSON :: Feature&
Dataset::Place ( Id p_id ) const  throw ( std :: invalid_argument )
{
    return m_impl->Get(p_id);
}

Distance
Dataset::MaxDistance () const
{
    return m_impl->m_maxDistance;
}

const TextIndexReader&
Dataset::GetIndex () const
{
    return *m_impl->m_reader;
}

size_t
Dataset::FeatureCount () const
{
    return m_impl->Count();
}

//TODO: all the methods below
const Response&
Dataset::Search ( const char* text,
                    const BoundingBox& scope,
                    Format format )
{
    throw logic_error ( "not implemented" );
}

const Response&
Dataset::Autocomplete ( const char* text, const LatLon& scope )
{
    throw logic_error ( "not implemented" );
}

const Response&
Dataset::Autocomplete ( const char* text, const BoundingBox& scope )
{
    throw logic_error ( "not implemented" );
}

const Response&
Dataset::Reverse ( const LatLon& point,
                     const Distance& radius,
                     Format format )
{
    throw logic_error ( "not implemented" );
}

