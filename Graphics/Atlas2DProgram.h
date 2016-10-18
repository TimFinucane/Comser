#pragma once

#include <Vector.h>
#include <Matrix.h>

#include "Shader.h"
#include "Program.h"
#include "Mesh.h"
#include "BufferInfo.h"
#include "TextureAtlas.h"

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
    public:
        Atlas2DProgram( /* shaders filenames here */ );

        void    create( Graphics::Shaders::Shader shaders[2] );

        static IndexedMesh square( double width, double height )
        {
            return IndexedMesh( 
                IndexedMeshData<Vertex>( 
                {
                    { -width / 2.0,  height / 2.0, 0.0, 0.0 },
                    { -width / 2.0, -height / 2.0, 0.0, 1.0 },
                    {  width / 2.0,  height / 2.0, 1.0, 0.0 },
                    {  width / 2.0, -height / 2.0, 1.0, 1.0 }
                },
                {
                    0, 1, 3,
                    3, 2, 0
                } ) 
            );
        }

        void    add( Mesh& mesh, Maths::Vector2D vector, Maths::Matrix2& mat, unsigned int layer );

        void    draw( TextureAtlas& atlas );
    private:
        unsigned int    _vertexPos;
        unsigned int    _texCoordPos;
        unsigned int    _posUniform;
        unsigned int    _matUniform;

        Graphics::Shaders::IndexedBufferInfo    _array;
    };
}