#ifndef Mapzen_Pelopia_LocationFilter_h
#define Mapzen_Pelopia_LocationFilter_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* LocationFilter, a bitmap representation of a set of locations falling into a coordinate segment
*
*/

namespace Mapzen
{
	namespace Pelopia
	{

        class LocationFilter
        {
        public:
            LocationFilter ( size_t count );
            virtual ~LocationFilter() {};

            void Set ( size_t p_index );
            void Unset ( size_t p_index );
            bool IsSet ( size_t p_index ) const;

            LocationFilter& And ( const LocationFilter& );
        };

    }
}

#endif

