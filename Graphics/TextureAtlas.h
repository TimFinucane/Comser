#pragma once

#include <istream>

#include "Image.h"

namespace Graphics
{
    class TextureAtlas
    {
    public:
        TextureAtlas( const Image& file, unsigned int layerWidth, unsigned int layerHeight, unsigned int xLayers, unsigned int yLayers );
        ~TextureAtlas()
        {
            clear();
        }

        TextureAtlas( TextureAtlas&& tex ) = default;
        TextureAtlas& operator =( TextureAtlas&& tex ) = default;

        void            clear();

        void            bind() const;

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