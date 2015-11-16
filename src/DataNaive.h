#ifndef Mapzen_Pelopia_DataNaive_h
#define Mapzen_Pelopia_DataNaive_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* DataNaive, a simplistic implementation of Data, a collection of geo objects subject to search via Dataset.
*
* Reads GeocodeJSON from a text file, inflates all objects and places them into a vector.
*
*/

#include "Data.h"

namespace Mapzen
{
	namespace Pelopia
	{
        class DataNaive : public Data
        {
        public:
            DataNaive ( const std :: string& filename ) throw ( std :: invalid_argument );
            
            virtual ~DataNaive();
            
            virtual const GeocodeJSON::Feature& Get ( Id ) const;
            
            virtual size_t Count () const;
        };
    }
}

#endif

