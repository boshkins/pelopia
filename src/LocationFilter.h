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

#include <pelopia/Pelopia.h>

#include <vector>

namespace Mapzen
{
	namespace Pelopia
	{

        class LocationFilter
        {
        public:
            LocationFilter ( Id maxId ); // sets all to 0
            virtual ~LocationFilter() {};

            Id MaxId () const;

            // IDs are 1-based
            void Set ( Id p_id );
            void Unset ( Id p_id );
            bool IsSet ( Id p_id ) const;

            LocationFilter& And ( const LocationFilter& );

        private:
            void CheckId ( Id p_id ) const;

            std::vector<bool> m_body;
        };

    }
}

#endif

