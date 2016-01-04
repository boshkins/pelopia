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

#include <vector>

#include "Data.h"

namespace Mapzen
{
	namespace Pelopia
	{
        namespace GeocodeJSON
        {
            class Feature;
            class Reader_Rapid_DOM;
        } 
        
        class DataNaive : public Data
        {
        public:
            // reads from an external GeocodeJSON file using RapidJSON based DOM reader 
            DataNaive ( const std :: string& filename ) throw ( std :: invalid_argument, std :: logic_error );
            virtual ~DataNaive();
            
            virtual const GeocodeJSON :: Feature& Get ( Id ) const throw ( std :: invalid_argument );
            
            virtual size_t Count () const;
            
         private:
            typedef std :: vector < GeocodeJSON :: Feature* > Features;
            Features m_features;
            
            void Clear();
            
            GeocodeJSON::Reader_Rapid_DOM* m_reader;
        };
    }
}

#endif
