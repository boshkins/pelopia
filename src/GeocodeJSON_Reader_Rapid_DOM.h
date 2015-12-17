#ifndef Mapzen_Pelopia_GeocodeJSON_Reader_Rapid_DOM_h
#define Mapzen_Pelopia_GeocodeJSON_Reader_Rapid_DOM_h

/*
* Copyright notice: TBD
*/

/*
* Implementation of GeocodeJSON_Reader, using RapidJSON and DOM mode
*/

#include "GeocodeJSON_Reader.h"

#include "rapidjson/document.h"

#include <iostream>

namespace Mapzen
{
namespace Pelopia
{
namespace GeocodeJSON
{
    class Reader_Rapid_DOM : public Reader
    {
    public:
        Reader_Rapid_DOM ( std :: istream& )  throw ( std :: logic_error );
        
        virtual ~Reader_Rapid_DOM ();
        
        virtual Feature* NextValue () throw ( std :: logic_error ); 
        
    private:
        Reader_Rapid_DOM ( const Reader_Rapid_DOM& );
        Reader_Rapid_DOM& operator = ( const Reader_Rapid_DOM& );
        
    private:
        class StlStream;
        class FeatureDOM;
        
    private:
        rapidjson :: Document   m_doc;
        rapidjson :: Value      m_features;
        rapidjson :: SizeType   m_curFeature;
    };
}
}
}    

#endif
