#include "CoordinateIndex.h"

#include <map>

using namespace Mapzen :: Pelopia;
using namespace std;

///////////// CoordinateIndex::Impl

typedef multimap < Coordinate, Id > IndexImpl;

class CoordinateIndex::Impl : public IndexImpl
{
public:
    void Insert( Coordinate p_coord, Id p_id )
    {
        insert ( value_type ( p_coord, p_id ) );
    }

    ConstIterator FindSegment ( Coordinate p_low, Coordinate p_high ) const
    {
        return ConstIterator(*this, p_low, p_high);
    }
};

///////////// CoordinateIndex::ConstIterator::Impl
class CoordinateIndex::ConstIterator::Impl
{
public:
    Impl ( CoordinateIndex::Impl::const_iterator p_begin,
           CoordinateIndex::Impl::const_iterator p_end )
    :   m_begin ( p_begin ),
        m_end ( p_end )
    {
    }

    bool operator == ( const Impl& p_that ) const
    {
        return m_begin == p_that.m_begin && m_end == p_that.m_end;
    }

    CoordinateIndex::Impl::const_iterator m_begin;
    CoordinateIndex::Impl::const_iterator m_end;
};

///////////// CoordinateIndex::ConstIterator

CoordinateIndex::ConstIterator::ConstIterator ( const CoordinateIndex::Impl& p_index, Coordinate p_first, Coordinate p_last )
: m_impl ( nullptr )
{
    if ( p_first <= p_last )
    {
        CoordinateIndex::Impl::const_iterator firstOrGreater = p_index.lower_bound(p_first);
        if (firstOrGreater != p_index.end())
        {
            CoordinateIndex::Impl::const_iterator greaterThanLast = p_index.upper_bound(p_last);
            if ( firstOrGreater != greaterThanLast )
            {
                m_impl = new Impl ( firstOrGreater, greaterThanLast );
            }
        }
    }
}

CoordinateIndex::ConstIterator::ConstIterator ()
: m_impl ( nullptr )
{
}

CoordinateIndex::ConstIterator::ConstIterator ( const ConstIterator& p_that )
: m_impl ( p_that.m_impl == nullptr ? nullptr : new Impl ( *p_that.m_impl ) )
{
}

CoordinateIndex::ConstIterator::~ConstIterator ()
{
    delete m_impl;
}

bool
CoordinateIndex::ConstIterator::operator == ( const ConstIterator& p_that ) const
{
    if ( m_impl == nullptr )
    {
        return p_that.m_impl == nullptr;
    }
    if ( p_that.m_impl == nullptr )
    {
        return false;
    }
    return *m_impl == *p_that.m_impl;
}

Id
CoordinateIndex::ConstIterator::operator* () const
{
    if (m_impl == nullptr)
    {
        return InvalidId;
    }
    return m_impl->m_begin->second;
}

CoordinateIndex::ConstIterator
CoordinateIndex::ConstIterator::operator ++ ()
{
    if (m_impl != nullptr)
    {
        ++m_impl->m_begin;

        if (m_impl->m_begin == m_impl->m_end)
        {
            m_impl = nullptr;
        }
    }
    return *this;
}

CoordinateIndex::ConstIterator
CoordinateIndex::ConstIterator::operator ++ (int) // postfix
{
    ConstIterator ret = *this;
    ++ *this;
    return ret;
}

///////////// CoordinateIndex

CoordinateIndex::CoordinateIndex()
: m_impl ( new Impl() )
{
}

CoordinateIndex::~CoordinateIndex()
{
    delete m_impl;
}

void
CoordinateIndex::Insert( Coordinate p_coord, Id p_id )
{
    m_impl->Insert ( p_coord, p_id );
}

CoordinateIndex::ConstIterator
CoordinateIndex::FindSegment ( Coordinate p_low, Coordinate p_high ) const
{
    return m_impl->FindSegment ( p_low, p_high );
}

CoordinateIndex::ConstIterator
CoordinateIndex::end() const
{
    return ConstIterator();
}

