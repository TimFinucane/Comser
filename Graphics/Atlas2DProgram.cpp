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
}

void Atlas2DProgram::create( Graphics::Shaders::Shader shaders[2] )
{
    std::string error;
    Program::create( shaders, 2, error );

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