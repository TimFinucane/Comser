#pragma once

#include <istream>

#include "Image.h"

namespace Graphics
{
    class TextureAtlas
    {
    public:
        TextureAtlas( const Image::File& file, unsigned int width, unsigned int height, unsigned int layers );
        ~TextureAtlas()
        {
            clear();
        }

        TextureAtlas( TextureAtlas&& tex ) = default;
        TextureAtlas& operator =( TextureAtlas&& tex ) = default;

        void            clear();

        void            bind();

        unsigned int    width()
        {
            return _width;
        }
        unsigned int    height()
        {
            return _height;
        }
        unsigned int    layers()
        {
            return _layers;
        }

        // For use by graphics shaders
        unsigned int    texture()
        {
            return _texture;
        }
    private:
        unsigned int    _texture;

        unsigned int    _width;
        unsigned int    _height;
        unsigned int    _layers;
    };
}