#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Image.h"

namespace Graphics
{
    // This is a texture itself.
    struct Texture
    {
    public:
        Texture();
        ~Texture();

        Texture( Texture& tex ) = delete;
        void operator =( Texture& tex ) = delete;

        Texture( Texture&& tex ) = default;
        Texture& operator =( Texture&& tex ) = default;

        void    load( const Image::File& file );

        void    bind();
    private:
        unsigned int _texture;
    };
}

#endif