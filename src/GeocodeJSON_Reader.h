#ifndef Mapzen_Pelopia_GeocodeJSON_Reader_h
#define Mapzen_Pelopia_GeocodeJSON_Reader_h

/*
* Copyright notice: TBD
*/

/*
* Pelopia :: GeocodeJSON_Reader, an interface to read GeocodeJSON values
*/

#include <pelopia/GeocodeJSON.h>

#include <stdexcept>

namespace Mapzen {
namespace Pelopia {
namespace GeocodeJSON 
{

    class Reader
    {
    public:
        virtual ~Reader () = 0;
        
        // returns 0 at the end of the input
        // calls past the end the input will throw
        // any exceptions thrown by the underlying stream will be propagated
        // objects not conforming to JSON syntax will throw 
        // objects not conforming to GeocodeJSON will throw 
        virtual const Feature* NextValue () throw ( std :: logic_error ) = 0 ; 
    };
    
} } }

#endif
