#include "Atlas2DProgram.h"

#include <GraphicsIncludes.h>

using namespace Graphics;

Atlas2DProgram::Atlas2DProgram()
    : _array( 2, sizeof( Vertex ) )
{
    _array.define( 0, offsetof( Vertex, Vertex::x ), GL_DOUBLE, 2 );
    _array.define( 1, offsetof( Vertex, Vertex::u ), GL_DOUBLE, 2 );

    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );

    // TODO: Find some better way of storing these
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
}

void Atlas2DProgram::create( const FileSystem::File& vertexFile, const FileSystem::File& fragmentFile )
{
    std::string error;
    Shaders::Shader vertex( GL_VERTEX_SHADER, vertexFile.file(), vertexFile.length(), error );
    Shaders::Shader fragment( GL_FRAGMENT_SHADER, vertexFile.file(), vertexFile.length(), error );

    Program::create( { &vertex, &fragment } );

    if( !error.empty() )
        throw std::runtime_error( error );

    _matUniform  = glGetUniformLocation( _program, "matrix" );
    _posUniform  = glGetUniformLocation( _program, "position" );
    _vertexPos   = glGetAttribLocation(  _program, "vertex" );
    _texCoordPos = glGetAttribLocation(  _program, "texCoord" );
}

void Atlas2DProgram::setPos( const Maths::Vector2D& pos )
{
    glUniform2f( _posUniform, pos.x(), pos.y() );
}
void Atlas2DProgram::setMatrix( const Maths::Matrix2& mat )
{
    glUniformMatrix2fv( _matUniform, 1, GL_FALSE, &mat[0] );
}

void Atlas2DProgram::draw()
{
    _array.bind( _vertexPos );

    glDrawElements( GL_TRIANGLES, _array.count(), GL_UNSIGNED_SHORT, nullptr );
}