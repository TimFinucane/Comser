#pragma once

#include <istream>

#include "Image.h"

namespace Graphics
{
    class SpriteSheet
    {
    public:
        SpriteSheet();
        ~SpriteSheet()
        {
            clear();
        }

        SpriteSheet( SpriteSheet&& tex ) = default;
        SpriteSheet& operator =( SpriteSheet&& tex ) = default;

        void            load( const Image::File& file, unsigned int width, unsigned int height, unsigned int layers );
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
    private:
        unsigned int    _texture;

        unsigned int    _width;
        unsigned int    _height;
        unsigned int    _layers;
    };
}