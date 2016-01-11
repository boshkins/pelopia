#ifndef Mapzen_Pelopia_Response_h
#define Mapzen_Pelopia_Response_h

/*
* Copyright notice: TBD
*/

/*
* API for Pelopia, an open source offline geocoding library
* Project repository: https://github.com/boshkins/pelopia
*/

#include <pelopia/Pelopia.h>

#include <string>
#include <vector>

namespace Mapzen
{
	namespace Pelopia
	{

		class Response
		{	// a collection of results, sorted by descending quality
        public:
            Response ( unsigned int reserve = DefaultResults );
            
            Response ( const Response& );
            
            Response& operator = ( const Response& );
            
		public:
			// the number of entries ion the response
			unsigned int Count() const;

			// access location Id and its score by its position in the collection, 
			//		0 <= index < Count
			// returns false if index is outside of [ 0, Count ) 
			bool Get ( unsigned int index, Id& id, MatchQuality& score ) const;

			// access location Id by position in the collection, 0 <= index < Count
			// returns InvalidId if index is outside of [ 0, Count )
			Id Get ( unsigned int index ) const;

			// access the best-scoring completion for the corresponding entry's text, 
			// if requested in the original query (i.e. Dataset :: Autocomplete called)
			// returns empty string if index is outside of [ 0, Count )
			std :: string Autocomplete ( unsigned int index ) const;

        protected:
            typedef struct Match 
            {
                Id id;
                MatchQuality score;
            } Match;
            typedef std :: vector < Match > Matches;
            
            Matches m_matches;
		};

	}
}

#endif
