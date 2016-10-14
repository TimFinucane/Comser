#pragma once

#include <Vector.h>
#include <Matrix.h>

#include "Shader.h"
#include "Program.h"
#include "Mesh.h"
#include "BufferArray.h"

// Uses a 2D texture
namespace Graphics
{
    class Atlas2DProgram : public Graphics::Shaders::Program
    {
        struct Vertex
        {
            double x;
            double y;

            double u;
            double v;
        };

        typedef Mesh<Vertex>    Mesh;
    public:
        Atlas2DProgram( /* shaders filenames here */ );

        void    create( Graphics::Shaders::Shader shaders[2] );

        static void square( double x, double y, Vertex in[4] )
        {
            in[0] ={ -x / 2.0f, y / 2.0f,  0.0f, 0.0f };
            in[1] ={ -x / 2.0f, -y / 2.0f, 0.0f, 1.0f };
            in[2] ={ x / 2.0f,  y / 2.0f, 1.0f, 0.0f };
            in[3] ={ x / 2.0f, -y / 2.0f, 1.0f, 1.0f };
        }
        static void squareIndices( unsigned short indices[6] )
        {
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 3;

            indices[3] = 3;
            indices[4] = 2;
            indices[5] = 0;
        }

        void    add( Mesh& mesh, Maths::Vector2D vector, Maths::Matrix2& mat, unsigned int layer );

        void    draw();
    private:
        unsigned int    _vertexPos;
        unsigned int    _texCoordPos;
        unsigned int    _posUniform;
        unsigned int    _matUniform;

        Graphics::Shaders::IndexedBufferArray  _array;
    };
}