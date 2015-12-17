#ifndef Mapzen_Pelopia_ResponseInternal_h
#define Mapzen_Pelopia_ResponseInternal_h

/*
* Copyright notice: TBD
*/

/*
* Internal writeable extension of Response 
*/

#include <pelopia/Response.h>

namespace Mapzen
{
	namespace Pelopia
	{
        // writeable extension of Response
		class ResponseInternal : public Response
		{ 
        public:
            ResponseInternal ( unsigned int p_reserve );
            
            void AppendMatch ( Id id, MatchQuality score );
            void AppendAutocomplete ( const std::string& );
		};

	}
}

#endif
