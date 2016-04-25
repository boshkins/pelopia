#include "CoordinateIndex.h"

#include <map>

using namespace Mapzen :: Pelopia;
using namespace std;

///////////// CoordinateIndex::ConstIterator::Impl
class CoordinateIndex::ConstIterator::Impl
{
public:
    Impl() {}
    Impl(const Impl&) {}
    ~Impl() {}

    bool operator == (const Impl&) const { return false; }
};

///////////// CoordinateIndex::ConstIterator

CoordinateIndex::ConstIterator::ConstIterator ( CoordinateIndex::Impl&, Coordinate first, Coordinate last )
: m_impl ( nullptr )
{
}

CoordinateIndex::ConstIterator::ConstIterator ()
: m_impl ( nullptr )
{
}

CoordinateIndex::ConstIterator::ConstIterator ( const ConstIterator& p_that )
: m_impl ( new Impl ( *p_that.m_impl ) )
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

///////////// CoordinateIndex::Impl

class CoordinateIndex::Impl
{
public:
    Impl()
    {
    }

    ~Impl()
    {
    }

    void Insert( Coordinate p_coord, Id p_id )
    {
        m_index.insert ( Index::value_type ( p_coord, p_id ) );
    }

    Id At ( size_t p_index ) const
    {

    }

    void FindSegment ( Coordinate p_low, Coordinate p_high, size_t& p_first, size_t& p_last ) const;

private:
    typedef multimap < Coordinate, Id > Index;

    Index m_index;
};

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
    return ConstIterator ( *m_impl, p_low, p_high );
}

CoordinateIndex::ConstIterator
CoordinateIndex::end() const
{
    return ConstIterator();
}
