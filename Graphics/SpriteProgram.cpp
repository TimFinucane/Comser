#include "SpriteProgram.h"

#include <GraphicsIncludes.h>

using namespace Graphics;

SpriteProgram::SpriteProgram( const FileSystem::File& vertexFile, const FileSystem::File& fragmentFile )
    : Program{ Shaders::Shader( GL_VERTEX_SHADER, vertexFile ), Shaders::Shader( GL_FRAGMENT_SHADER, fragmentFile ) },
        _array( 1, sizeof( Sprite ) )
{
    _array.define( 0, 0, GL_DOUBLE, 2 );

    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );

    // TODO: Find some better way of storing these
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    //_matUniform = glGetUniformLocation( _program, "matrix" );
    //_posUniform = glGetUniformLocation( _program, "position" );
    //_vertexPos = glGetAttribLocation( _program, "vertex" );
    //_texCoordPos = glGetAttribLocation( _program, "texCoord" );
}

void SpriteProgram::draw( Sprites& sprites, const TextureAtlas& atlas )
{
    _array.bind( sprites, _vertexPos );

    glDrawArrays( GL_POINTS, 0, sprites.size() );
}