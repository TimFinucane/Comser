#include "TextureAtlas.h"

#include <GraphicsIncludes.h>


using namespace Graphics;

TextureAtlas::TextureAtlas( const Image::File& file, unsigned int width, unsigned int height, unsigned int layers )
{
    glGenTextures( 1, &_texture );

    // TODO: If feeling kind, store and then restore anything currently bound to GL_TEXTURE_2D_ARRAY. Should be pointless though
    glBindTexture( GL_TEXTURE_2D_ARRAY, _texture );

    glTexStorage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layers );
    glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layers, GL_RGBA, GL_UNSIGNED_BYTE, file.file() );

    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
}
void    TextureAtlas::clear()
{
    glDeleteTextures( 1, &_texture );
}