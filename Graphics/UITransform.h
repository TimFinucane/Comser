#pragma once

#pragma warning( push )
#pragma warning( disable : 4634 )
#pragma warning( disable : 4635 )
#pragma warning( disable : 4201 )
#include <glm/matrix.hpp>
#pragma warning( pop )

namespace UI
{
    struct Transform
    {
        const glm::mat4&    operator()() const
        {
            return matrix;
        }
        glm::mat4&          operator()()
        {
            return matrix;
        }

        glm::mat4   matrix;
    };
}