#pragma once

namespace UI
{
    struct RenderState;
    struct Transform;

    class View
    {
    public:
        virtual void    render( RenderState& renderState, const Transform& transform ) = 0;
    };
}