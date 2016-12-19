#pragma once

#include "BasicProgram.h"

namespace UI
{
    struct RenderState
    {
        template <typename... ARGS>
        RenderState( ARGS... args )
            : program( std::forward<ARGS>( args )... )
        {
        }

        Graphics::BasicProgram program;
    };
}