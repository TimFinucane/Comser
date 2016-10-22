#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <string>
#include <map>

#include "Image.h"

class Window;

namespace Graphics
{
    class Texture;

    namespace Text
    {
        struct Line
        {
            // The image (in single-channel 8-bit alpha) that can be directly converted to a texture.
            // Also contains width and height of file
            Image file;
            // Distance between text baseline and the actual top of the image
            unsigned int cntrHeight;
        };
        struct Paragraph
        {
            Image file;
        };

        // A specific font
        class Font
        {
            friend class Library;

            static const unsigned int MAX_SIZES = 3;

            typedef unsigned int GlyphIndex;
            struct GlyphInfo
            {
                GlyphInfo( FT_Face face, GlyphIndex gi )
                {
                    FT_Load_Glyph( face, gi, FT_LOAD_DEFAULT );

                    FT_Get_Glyph( face->glyph, reinterpret_cast<FT_Glyph*>(&glyph) );

                    FT_Glyph_To_Bitmap( (FT_Glyph*)&glyph, FT_RENDER_MODE_LIGHT, nullptr, true );
                }
                ~GlyphInfo()
                {
                    FT_Done_Glyph( (FT_Glyph)glyph );
                }

                FT_BitmapGlyph  glyph;
            };
            typedef std::map<GlyphIndex, GlyphInfo>     GlyphMap;

            class FontRenderer;
        public:
            typedef unsigned int UChar;

            enum Type
            {
                DEFAULT,
                ITALICS,
                BOLD
            };
        public:
            Font( FT_Library lib, unsigned char* file, unsigned int length, unsigned int face );
            ~Font();

            // Set the font size based on text height (in pixels)
            void            useSize( unsigned int pxHeight );

            Line            genLine( UChar* text, unsigned int length, bool kern );
            Paragraph       genParagraph( UChar* text, unsigned int length, unsigned int width );
        private:
            inline GlyphMap&        _glyphMap()
            {
                return _glyphMaps[_curSize];
            }

            GlyphMap::iterator      _cacheGlyph( GlyphIndex gi )
            {
                GlyphMap::iterator it = _glyphMap().find( gi );
                if( it == _glyphMap().end() )
                    it = _glyphMap().emplace( std::piecewise_construct,
                                              std::forward_as_tuple( gi ),
                                              std::forward_as_tuple( _face, gi ) ).first;

                return it;
            }

            FT_Face         _face;

            unsigned int    _curSize;
            GlyphMap        _glyphMaps[MAX_SIZES];
        }; 
    }
}