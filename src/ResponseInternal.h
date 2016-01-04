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
            ResponseInternal ( unsigned int p_reserve = DefaultResults );
            
            void Clear();
            
            void AppendMatch ( Id id, MatchQuality score );
            void AppendAutocomplete ( const std::string& );
            
            void Sort();
            void Truncate(size_t p_members);
            
        private:
            static bool Greater ( const Response::Match&, const Response::Match& );
		};

	}
}

#endif
