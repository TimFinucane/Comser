#include "TextureAtlas.h"

#include <GraphicsIncludes.h>


using namespace Graphics;

TextureAtlas::TextureAtlas( const Image& file, unsigned int layerWidth, unsigned int layerHeight, unsigned int xLayers, unsigned int yLayers )
    : _width( layerWidth ), _height( layerHeight ), _layers( xLayers * yLayers )
{
    glGenTextures( 1, &_texture );

    // TODO: If feeling kind, store and then restore anything currently bound to GL_TEXTURE_2D_ARRAY. Should be pointless though
    glBindTexture( GL_TEXTURE_2D_ARRAY, _texture );

    unsigned char* tex = new unsigned char[file.size()];
    unsigned int rowSize = (unsigned int)( layerWidth * file.pixelDepth() / 8.0f );

    unsigned int atlasPos = 0;
    for( unsigned int yStart = 0; yStart < yLayers; ++yStart )
        for( unsigned int xStart = 0; xStart < xLayers; ++xStart )
            for( unsigned int y = 0; y < layerHeight; ++y )
            {
                memcpy( tex + atlasPos, file.file( xStart * layerWidth, yStart * layerHeight + y ), rowSize );
                atlasPos += rowSize;
            }

    glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, _width, _height, xLayers * yLayers, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex );
    
    delete[] tex;

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