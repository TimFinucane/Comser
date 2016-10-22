#include "TextureAtlas.h"

#include <GraphicsIncludes.h>


using namespace Graphics;

TextureAtlas::TextureAtlas( const Image& file, unsigned int width, unsigned int height, unsigned int layers )
    : _width( width ), _height( height ), _layers( layers )
{
    glGenTextures( 1, &_texture );

    // TODO: If feeling kind, store and then restore anything currently bound to GL_TEXTURE_2D_ARRAY. Should be pointless though
    glBindTexture( GL_TEXTURE_2D_ARRAY, _texture );

    glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, file.file() );

    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
}
void    TextureAtlas::clear()
{
    glDeleteTextures( 1, &_texture );
}

void    TextureAtlas::bind() const
{
    glBindTexture( GL_TEXTURE_2D_ARRAY, _texture );
}