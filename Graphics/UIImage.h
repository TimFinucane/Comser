#pragma once

#include "Texture.h"

#include "UIView.h"

namespace UI
{
    class Image : public View
    {
    public:
        Image()
        {
        }
        Image( Graphics::Texture&& texture )
            : texture( std::move( texture ) )
        {
        }

        // Move operators should be automatically made

        void    render( RenderState& renderState, const Transform& transform );

        Graphics::Texture   texture;
    };
}