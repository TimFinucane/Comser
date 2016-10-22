#include "Texture.h"

#include <GraphicsIncludes.h>

using namespace Graphics;

Texture::Texture( const Image& file )
{
    //glCreateTextures( GL_TEXTURE_2D, 1, &texture ); 4.1 and above only :(
    glGenTextures( 1, &_texture );

    // TODO: If feeling kind, store and then restore anything currently bound to GL_TEXTURE_2D_ARRAY. Should be pointless though
    glBindTexture( GL_TEXTURE_2D, _texture );

    unsigned int format;

    switch( file.channels() )
    {
    case 1:
        format = GL_ALPHA;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
    default:
        format = 0;
        throw std::runtime_error( "Error with file format: Need 1, 3, or 4 channels to create OpenGL Image" );
        break;
    }

    if( file.bitDepth() != 8 )
        throw; //TODO: Support more formats

    glTexStorage2D( GL_TEXTURE_2D, 0, format, file.width(), file.height() );
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, file.width(), file.height(), format, GL_UNSIGNED_BYTE, file.file() );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
}
Texture::~Texture()
{
    glDeleteTextures( 1, &_texture );
}

void Texture::bind()
{
    glBindTexture( GL_TEXTURE_2D, _texture );
}