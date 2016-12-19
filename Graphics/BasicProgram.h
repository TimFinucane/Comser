#pragma once

#pragma warning( push )
#pragma warning( disable : 4634 )
#pragma warning( disable : 4635 )
#pragma warning( disable : 4201 )
#include <glm/gtc/matrix_transform.hpp>
#pragma warning( pop )

#include <File.h>

#include "Program.h"
#include "BufferInfo.h"
#include "Texture.h"
#include "ObjectArray.h"

namespace Graphics
{
    class BasicProgram : public Graphics::Shaders::Program
    {
    public:
        BasicProgram( const FileSystem::File& vertex, const FileSystem::File& fragment );
        ~BasicProgram() {}

        void    draw( const glm::mat4& matrix, const Texture& texture );

    private:
        struct Vertex
        {
            float xPos;
            float yPos;

            float xTex;
            float yTex;
        };

        unsigned int                            _modelProjPos;

        ObjectArray<Vertex>                     _vertices{ Buffer::UpdateFrequency::ONCE };
        ObjectArray<unsigned short>             _indices{ Buffer::UpdateFrequency::ONCE };

        Graphics::Shaders::IndexedBufferInfo    _squareInfo;
    };
}