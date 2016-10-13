#pragma once

#include <Vector.h>
#include <Matrix.h>

#include "Shader.h"
#include "Program.h"
#include "BufferArray.h"

struct Generic2DVertex
{
    static void square( float x, float y, Generic2DVertex in[4] )
    {
        in[0] = { -x / 2.0f, y / 2.0f,  0.0f, 0.0f };
        in[1] = { -x / 2.0f, -y / 2.0f, 0.0f, 1.0f };
        in[2] = {  x / 2.0f,  y / 2.0f, 1.0f, 0.0f };
        in[3] = {  x / 2.0f, -y / 2.0f, 1.0f, 1.0f };
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

    float x;
    float y;

    float u;
    float v;
};

// Uses a 2D texture
class Generic2DProgram : public Graphics::Shaders::Program
{
public:
    Generic2DProgram();

    void    create( Graphics::Shaders::Shader shaders[2] );

    void    setPos( const Maths::Vector2D& pos );
    void    setMatrix( const Maths::Matrix2& mat );

    void    draw();
private:
    unsigned int    _vertexPos;
    unsigned int    _texCoordPos;
    unsigned int    _posUniform;
    unsigned int    _matUniform;

    Graphics::Shaders::IndexedBufferInfo  _array;
};