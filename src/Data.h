#ifndef Mapzen_Pelopia_Data_h
#define Mapzen_Pelopia_Data_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Internal interface for Data, a collection of geo objects subject to search via Dataset
*/

#include <stdexcept>

#include <pelopia/GeocodeJSON.h>

namespace Mapzen
{
	namespace Pelopia
	{
        class Data
        {
        public:
            virtual ~Data() = 0;
            
            virtual const GeocodeJSON::Feature& Get ( Id ) const = 0;
            
            virtual size_t Count () const = 0;
        };
    }
}

#endif

