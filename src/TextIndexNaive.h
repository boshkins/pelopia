#ifndef Mapzen_Pelopia_TextIndexNaive_h
#define Mapzen_Pelopia_TextIndexNaive_h

/*
* Copyright notice: TBD
*/

/*
* Project repository: https://github.com/boshkins/pelopia
*
* TextIndexNaiveWriter/Reader, an STL-based implementation index into terms used in a Dataset
*
*/

#include "TextIndex.h"

namespace Mapzen
{
	namespace Pelopia
	{
        class TextIndexNaive;

        class TextIndexNaiveWriter : public TextIndexWriter
        {
        public:
            TextIndexNaiveWriter();
            virtual ~TextIndexNaiveWriter();

            virtual void AddTermUse ( const std :: string& term,
                                      Id object,
                                      GeocodeJSON :: Feature :: SearchablePropertyId,
                                      size_t index,
                                      size_t offset );

            TextIndexNaive* DetachIndex(); // ownership transferred to the caller

        private:
            TextIndexNaive* m_index;
        };

        class TextIndexNaiveReader : public TextIndexReader
        {
        public:
            TextIndexNaiveReader ( TextIndexNaive* ); // assumes ownership of the index

            virtual ~TextIndexNaiveReader();

            virtual const Node* Locate ( const char32_t* prefix ) const;

        private:
            TextIndexNaive* m_index;
        };

    }
}

#endif

