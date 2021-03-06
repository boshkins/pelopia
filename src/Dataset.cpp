#include <pelopia/Dataset.h>

#include <stdexcept>
#include <map>
#include <limits>

#include "GeocodeJSON_Reader_Rapid_DOM.h"
#include "DataNaive.h"
#include "TextIndexNaive.h"
#include "NormalizerNaive.h"
#include "ResponseInternal.h"
#include "GeoScorer.h"
#include "TextScorer.h"
#include "FilteringGeoTextScorer.h"
#include "CoordinateIndex.h"
#include "LocationFilter.h"

using namespace Mapzen :: Pelopia;
using namespace std;

///////////// Dataset::Impl

class Dataset::Impl : public DataNaive
{
public:
    Impl( const char* p_filename ) throw( std :: invalid_argument, std :: logic_error )
    :   DataNaive( p_filename ),
        m_maxDistance( Distance::Kilometers, 0 )
    {
        TextIndexNaiveWriter w;

        // traverse data, add all terms to writer, calculate max distance
        Coordinate minLat   = numeric_limits<Coordinate>::max();
        Coordinate minLon   = numeric_limits<Coordinate>::max();
        Coordinate maxLat   = numeric_limits<Coordinate>::lowest();
        Coordinate maxLon   = numeric_limits<Coordinate>::lowest();

        size_t count = DataNaive::Count();
        for( size_t i = 0 ; i < count; ++i )
        {   // iterate over all searchable properties in Get(i+1), split into terms, normalize, add to writer
            // calculate the maximum distance between features ion the dataset( minLat,minLon to maxLat,maxLon )
            Id id = i + 1;

            const GeocodeJSON :: Feature& feature = DataNaive::Get( id );

            for( GeocodeJSON :: Feature :: SearchablePropertyId property = GeocodeJSON :: Feature :: Property_begin;
                  property < GeocodeJSON :: Feature :: Property_end;
                  property =(GeocodeJSON :: Feature :: SearchablePropertyId)((unsigned int) property + 1) )
            {
                const char* value = feature.SearchableProperty( property );
                if( value != nullptr )
                {
                    Normalizer::Result res;
                    m_norm.Normalize( value, res );
                    for( auto&& it : res.m_terms )
                    {
                        w.AddTermUse( string( value + it.startBytes, it.lengthBytes ), //TODO( pass char32_t* it->m_norm ) to avoid an extra conversion from UTF8
                                       id,
                                       property,
                                       0,   // TODO: handle indexed properrties
                                       it.startBytes );
                    }
                }
            }

            // add to coord indexes
            m_pointLatIndex.Insert ( feature.GetGeometry()->Latitude(), id );
            m_pointLonIndex.Insert ( feature.GetGeometry()->Longitude(), id );

            // update min/max coords
            minLat = min( minLat, feature.GetGeometry()->Latitude() );
            minLon = min( minLon, feature.GetGeometry()->Longitude() );
            maxLat = max( maxLat, feature.GetGeometry()->Latitude() );
            maxLon = max( maxLon, feature.GetGeometry()->Longitude() );
        }

        m_reader = new TextIndexNaiveReader( w.DetachIndex() );
        m_maxDistance = LatLon( minLat, minLon ).DistanceTo( LatLon( maxLat, maxLon ) );
    }
    ~Impl()
    {
        delete m_reader;
    }

    typedef map<Id, MatchQuality> HitMap;

    const Response& Search( const Normalizer::Result& res, const Scorer& p_scorer, Format p_format )
    {
        HitMap hits;

        for( auto&& it : res.m_terms )
        {
            const TextIndexReader :: Node* node = m_reader->Locate( it.norm );
            if( node != nullptr )
            {   // add all uses of the term to the result set(filter out duplicates)
                const TextIndexReader :: Payload& uses = node->GetPayload();
                size_t count = uses.Count();
                for( size_t i = 0; i < count; ++i )
                {
                    Id id = uses.GetTermUse( i ).ObjectId();
                    MatchQuality score = p_scorer.Score( id );

                    if ( score > 0 )
                    {   // insert or update max score
                        HitMap::iterator hit = hits.find( id );
                        if( hit == hits.end() )
                        {
                            hits [ id ] = score;
                        }
                        else
                        {
                            hit->second = max( hit->second, score );
                        }
                    }
                }
            }

            // copy hits to the results container
            m_response.Clear();
            for( auto&& hit : hits )
            {
                m_response.AppendMatch( hit.first, hit.second );
            }

            // sort on descending score and exclude extra results
            m_response.Sort();
            m_response.Truncate( p_format.maxResults );
        }

        return m_response;
    }

    NormalizerNaive         m_norm;
    TextIndexNaiveReader*   m_reader;
    ResponseInternal        m_response;
    Distance                m_maxDistance;
    CoordinateIndex         m_pointLatIndex;
    CoordinateIndex         m_pointLonIndex;
};

///////////// Dataset

Dataset::Dataset( const char* filename ) throw( std :: invalid_argument, std :: logic_error )
{
    m_impl = new Impl( filename );
}

Dataset::~Dataset()
{
    delete m_impl;
}

const Response&
Dataset::Search( const char* p_text, Format p_format )
{
    Normalizer::Result res;
    m_impl->m_norm.Normalize( p_text, res );
    TextScorer ts( *this, m_impl->m_norm, res );
    return m_impl->Search( res, ts, p_format ); //TODO: will call Normalize again - refactor
}

const Response&
Dataset::Search( const char*        p_text,
                 const LatLon&      p_scope,
                 const Distance&    p_radius,
                 Format             p_format )
{
    Normalizer::Result res;
    m_impl->m_norm.Normalize( p_text, res );

    if ( p_radius.GetKilometers() == 0 )
    {
        return m_impl->Search( res, GeoTextScorer ( *this, p_scope, m_impl->m_norm, res ), p_format );
    }
    else
    {   // add a location filter

        // TODO: move into FilteringGeoTextScorer
        LocationFilter locationFilter ( m_impl -> Count() );
        BoundingBox bb ( p_scope, p_radius );
        for ( auto lat = m_impl->m_pointLatIndex . FindSegment ( bb.Bottom(), bb.Top() );
                lat != m_impl->m_pointLatIndex.end();
                ++lat )
        {
            locationFilter.Set ( *lat );
        }
        LocationFilter lonFilter ( m_impl -> Count() );
        for ( auto lon = m_impl->m_pointLonIndex . FindSegment ( bb.Left(), bb.Right() );
                lon != m_impl->m_pointLonIndex.end();
                ++lon )
        {
            lonFilter.Set ( *lon );
        }
        locationFilter.And(lonFilter);

        return m_impl->Search( res, FilteringGeoTextScorer ( *this, locationFilter, p_scope, m_impl->m_norm, res ), p_format );
    }
}

const GeocodeJSON :: Feature&
Dataset::Place( Id p_id ) const  throw( std :: invalid_argument )
{
    return m_impl->Get( p_id );
}

Distance
Dataset::MaxDistance() const
{
    return m_impl->m_maxDistance;
}

const TextIndexReader&
Dataset::GetIndex() const
{
    return *m_impl->m_reader;
}

size_t
Dataset::FeatureCount() const
{
    return m_impl->Count();
}

//TODO: all the methods below
const Response&
Dataset::Search( const char* text,
                    const BoundingBox& scope,
                    Format format )
{
    throw logic_error( "not implemented" );
}

const Response&
Dataset::Autocomplete( const char* text, const LatLon& scope )
{
    throw logic_error( "not implemented" );
}

const Response&
Dataset::Autocomplete( const char* text, const BoundingBox& scope )
{
    throw logic_error( "not implemented" );
}

const Response&
Dataset::Reverse( const LatLon& point,
                     const Distance& radius,
                     Format format )
{
    throw logic_error( "not implemented" );
}

