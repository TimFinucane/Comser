#include "Font.h"

#include FT_SIZES_H

#include <exception>
#include <vector>
#include <map>

#include <Window.h>

#define CHECK_ERROR(error, text) if( error ) throw std::runtime_error( text );

using namespace Graphics;
using namespace Graphics::Text;

Font::Font( FT_Library lib, unsigned char* file, unsigned int length, unsigned int face )
{
    FT_New_Memory_Face( lib, file, length, face, &_face );
    FT_Select_Charmap( _face, FT_ENCODING_UNICODE );

    useSize( 24 );
}
Font::~Font()
{
    FT_Done_Face( _face );
}

void    Font::useSize( unsigned int pxHeight )
{
    // See if we already have it
    for( unsigned int i = 0; i < (unsigned int)_face->num_fixed_sizes; ++i )
    {
        if( (unsigned int)_face->available_sizes->height == pxHeight )
        {
            FT_Select_Size( _face, i );
            _curSize = i;
            return;
        }
    }

    // Choose a size to replace with this one
    if( _face->num_fixed_sizes < MAX_SIZES  )
    {
        FT_Size size;
        FT_New_Size( _face, &size );
        FT_Select_Size( _face, _face->num_fixed_sizes - 1 );
        _curSize = _face->num_fixed_sizes - 1;
    }
    else
    {
        FT_Select_Size( _face, MAX_SIZES - 1 );
        _curSize = MAX_SIZES - 1;
    }

    FT_Set_Pixel_Sizes( _face, 0, pxHeight );
}

// TODO: Implement proper kerning
Line    Font::genLine( UChar* text, unsigned int length, bool kern )
{
    int width = 0;
    int highest = -10000;
    int lowest = 10000;
    
    // Map glyphs to each position they will appear in the text
    std::map<GlyphInfo*, std::vector<unsigned int>> glyphPositions;
    std::vector<unsigned int>   advances(length); // advances in x direction of each glyph from start

    // Load first value
    GlyphIndex gi = FT_Get_Char_Index( _face, text[0] );

    GlyphMap::iterator it = _cacheGlyph( gi );
    glyphPositions[&it->second].push_back( 0 );

    // TODO: Check if positive or negative
    width += it->second.glyph->left;
    advances[0] = width;

    // Check lowest and highest
    highest = highest > it->second.glyph->top ? highest : it->second.glyph->top;
    int low = it->second.glyph->top - it->second.glyph->bitmap.rows;
    lowest = lowest < low ? lowest : low;

    // Now load all other values
    GlyphIndex prev = gi;
    for( unsigned int i = 1; i < length; ++i )
    {
        gi = FT_Get_Char_Index( _face, text[i] );

        it = _cacheGlyph( gi );
        glyphPositions[&it->second].push_back( i );

        if( FT_HAS_KERNING( _face ) && kern )
        {
            FT_Vector advance;
            FT_Get_Kerning( _face, prev, gi, FT_KERNING_DEFAULT, &advance );
            width += advance.x;
        }
        else
            width += it->second.glyph->root.advance.x; // TODO: Check if its a .6 format

        advances[i] = advances[i - 1] + width;

        // Check lowest and highest
        highest = highest > it->second.glyph->top ? highest : it->second.glyph->top;
        low = it->second.glyph->top - it->second.glyph->bitmap.rows;
        lowest = lowest < low ? lowest : low;
    }

    width += it->second.glyph->bitmap.width - it->second.glyph->left;

    // And now write them all to the texture
    Line line;
    line.cntrHeight = highest;
    line.file = Image( width, highest - lowest, 8, 1 );

    for( auto jt = glyphPositions.begin(); jt != glyphPositions.end(); ++jt )
    {
        unsigned int topPos = highest - jt->first->glyph->top;
        unsigned int rows = jt->first->glyph->bitmap.rows;
        unsigned int cols = jt->first->glyph->bitmap.width;

        auto end = jt->second.end();
        for( auto pos = jt->second.begin(); pos != end; ++pos )
        {
            unsigned int middle = advances[*pos];

            // Write
            for( unsigned int y = 0; y < rows; ++y )
                for( unsigned int x = 0; x < cols; ++x )
                    *line.file.file( x + middle, topPos + y ) = jt->first->glyph->bitmap.buffer[y * rows + x];
        }

    }

    return line;
}