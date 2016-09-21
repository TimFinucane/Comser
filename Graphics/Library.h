#pragma once
#include <assert.h>
#include <string>
#include <map>

#include "Font.h"

namespace Graphics
{
    namespace Text
    {
        // A font file is an in-memory font that is passed to the 
        //  library. Can be destroyed once passed.
        struct FontFile
        {
            FontFile( void* file, unsigned int length, unsigned int face )
                : _length( length )
            {
                _file = new char[_length];
                memcpy_s( _file, _length, file, length );
            }
            ~FontFile()
            {
                clear();
            }

            FontFile( const FontFile& file )
            {
                clear();

                _length = file._length;
                _file = new char[file._length];
                memcpy_s( _file, _length, file._file, file._length );
            }
            FontFile& operator =( const FontFile& file )
            {
                clear();

                _length = file._length;
                _file = new char[file._length];
                memcpy_s( _file, _length, file._file, file._length );

                return *this;
            }

            FontFile( FontFile&& file ) noexcept = default;
            FontFile& operator =( FontFile&& file ) noexcept = default;

            void    clear()
            {
                if( _length > 0 )
                    delete[_length] _file;

                _length = 0;
                _file = nullptr;
            }

            const void*             file() const
            {
                return _file;
            }
            const unsigned int      length() const
            {
                return _length;
            }
            const unsigned int      face() const
            {
                return _face;
            }
        private:
            void*           _file;
            unsigned int    _length;
            unsigned int    _face;
        };

        // The library that controls the creation of fonts
        class Library
        {
            struct FontInfo
            {
                FontInfo( FT_Library lib, const FontFile& fontfile )
                    : _refs( 0 ), _marked( false ), 
                    _file( fontfile ), _face( lib, (unsigned char*)fontfile.file(), fontfile.length(), fontfile.face() )
                {
                }

                FontInfo( FontInfo&& inf ) noexcept = default;
                FontInfo& operator =( FontInfo&& inf ) noexcept = default;

                // Increments or decrements the reference
                void            ref( bool inc )
                {
                    assert( !(_refs == 0 && inc == false) );
                    _refs += inc;
                }
                unsigned int    refs() const
                {
                    return _refs;
                }

                // Marks the class itself for deletion
                void            mark()
                {
                    _marked = true;
                }
                bool            marked() const
                {
                    return _marked;
                }

                Font*           face()
                {
                    return &_face;
                }
            private:
                Font            _face;
                FontFile        _file;
                unsigned int    _refs;
                bool            _marked;
            };

            typedef std::map<std::string, FontInfo> FontMap;
            typedef FontMap::iterator               Reference;
        public:
            // Must be called before getting fonts
            void            init();
            // And call this at cleanup
            void            release();

            void            addFont( std::string name, const FontFile& file )
            {
                _fonts.try_emplace( name, _library, file );
            }
            void            destroyFont( std::string name )
            {
                auto it = _fonts.find( name );
                if( it == _fonts.end() )
                    return;

                if( it->second.refs() > 0 )
                {
                    it->second.mark();
                    return;
                }

                _fonts.erase( it );
            }

            Font*           useFont( std::string name, Reference* ref )
            {
                auto it = _fonts.find( name );
                if( it == _fonts.end() )
                    return nullptr;

                *ref = it;

                it->second.ref( true );

                return it->second.face();
            }
            void            unrefFont( Reference ref )
            {
                ref->second.ref( false );

                if( ref->second.refs() == 0 && ref->second.marked() )
                {
                    _fonts.erase( ref );
                }
            }
        private:
            FontMap         _fonts;
            FT_Library      _library;
        };
    }
}