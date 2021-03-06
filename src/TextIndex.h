#ifndef Mapzen_Pelopia_TextIndex_h
#define Mapzen_Pelopia_TextIndex_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* TextIndex, an index into terms used in a Dataset
*
*/

#include <pelopia/GeocodeJSON.h>

#include <string>

namespace Mapzen
{
	namespace Pelopia
	{

        class TextIndexWriter
        {
        public:
            virtual ~TextIndexWriter() {};

            virtual void AddTermUse ( const std :: string& term, //UTF-8
                                      Id object,
                                      GeocodeJSON :: Feature :: SearchablePropertyId,
                                      size_t index, // 0 if not array
                                      size_t offset ) = 0;
        };

        class TextIndexReader
        {
        public:
            typedef std :: invalid_argument index_out_of_bounds;

        public:
            class TermUse
            {
            public:
                virtual ~TermUse() {}

                virtual Id ObjectId () const = 0;
                virtual GeocodeJSON :: Feature :: SearchablePropertyId PropertyId () const = 0;
                virtual void Position ( size_t& index, size_t& offset, size_t& length ) const = 0;
            };

            class Payload
            {	// the collection is sorted by increasing object Id, then by increasing index, then by increasing position
            public:
                virtual ~Payload() {}

                virtual size_t Count () const = 0;
                virtual size_t FeatureCount () const = 0; // # of distinct feature Ids
                virtual const TermUse& GetTermUse ( size_t index ) const throw (index_out_of_bounds) = 0;
            };

            class Node
            {
            public:
                virtual ~Node() {}

                virtual const Payload& GetPayload () const = 0;

                virtual size_t ChildrenCount () const = 0;
                virtual const Node& GetChild ( size_t index ) const throw (index_out_of_bounds) = 0;
            };

        public:
            virtual ~TextIndexReader() {};

            // prefix in UTF-8
            virtual const Node* Locate ( const char32_t* prefix ) const = 0;
        };

    }
}

#endif

