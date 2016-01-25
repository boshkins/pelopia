#ifndef Mapzen_Pelopia_Scorer_h
#define Mapzen_Pelopia_Scorer_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* Internal interface for scoring features against a (normalized) query 
*/

#include <string>
#include <vector>

#include <Dataset.h>

namespace Mapzen
{
	namespace Pelopia
	{
        class Scorer
        {
        public:
            MatchQuality Score ( Dataset &, const std::vector < std::string > & query, const LatLon & center ) const;
        }
    }
}

#endif

