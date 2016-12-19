#pragma once

#include "UIGroup.h"
#include "UIRenderInfo.h"

namespace UI
{
    class Screen : public Group
    {
    public:
        Screen( RenderState& state, unsigned int width, unsigned int height )
            : renderState( state )
        {
            sizeChange( width, height );
        }
        
        void    render()
        {
            renderItems( renderState, transform );
        }

        void    sizeChange( unsigned int width, unsigned int height )
        {
            float aspect = width / (float)height;
            transform() = glm::ortho( -aspect, aspect, -1.0f, 1.0f );
        }

    private:
        RenderState& renderState;
        Transform   transform;
    };
}