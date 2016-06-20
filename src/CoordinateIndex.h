#ifndef Mapzen_Pelopia_CoordinateIndex_h
#define Mapzen_Pelopia_CoordinateIndex_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* CoordinateIndex, a collection of objectIds sorted by an increasing coordinate
*
*/

#include <pelopia/Pelopia.h>

namespace Mapzen
{
	namespace Pelopia
	{

        class CoordinateIndex
        {
        private:
            class Impl;

        public:
            class ConstIterator
            {
            public:
                ConstIterator( const ConstIterator& );
                ~ConstIterator();

                Id operator* () const;

                ConstIterator& operator = ( const ConstIterator& );

                bool operator == (const ConstIterator&) const;
                bool operator != (const ConstIterator& p_that) const { return ! operator == ( p_that ); }

                ConstIterator operator ++ ();
                ConstIterator operator ++ (int); // postfix

            private:
                friend class CoordinateIndex;

                ConstIterator( const CoordinateIndex::Impl&, Coordinate first, Coordinate last );
                ConstIterator(); // an end iterator

                class Impl;

                Impl* m_impl;
            };

        public:
            CoordinateIndex ();
            ~CoordinateIndex ();

            void Insert( Coordinate, Id );

            ConstIterator FindSegment ( Coordinate p_low, Coordinate p_high ) const;
            ConstIterator end () const;

        private:
            CoordinateIndex(const CoordinateIndex&);
            CoordinateIndex& operator=(const CoordinateIndex&);

            Impl* m_impl;
        };

    }
}

#endif

