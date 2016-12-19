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
        Texture()
            : _texture( 0 )
        {
        }
        Texture( const Image& file );
        ~Texture();

        Texture( Texture& tex ) = delete;
        void operator =( Texture& tex ) = delete;

        Texture( Texture&& tex )
        {
            _texture = tex._texture;
            tex._texture = 0;
        }
        Texture& operator =( Texture&& tex )
        {
            std::swap( _texture, tex._texture );

            return *this;
        }

        void    bind() const;
    private:
        unsigned int _texture;
    };
}

#endif