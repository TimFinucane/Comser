#include "Generic2DProgram.h"

#include <GraphicsIncludes.h>

Generic2DProgram::Generic2DProgram()
    : _array( 2, sizeof( Generic2DVertex ) )
{
    _array.define( 0, offsetof( Generic2DVertex, Generic2DVertex::x ), GL_FLOAT, 2 );
    _array.define( 1, offsetof( Generic2DVertex, Generic2DVertex::u ), GL_FLOAT, 2 );

    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
}

void Generic2DProgram::create( Graphics::Shaders::Shader shaders[2] )
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

void Generic2DProgram::setPos( const Maths::Vector2D& pos )
{
    glUniform2f( _posUniform, pos.x(), pos.y() );
}
void Generic2DProgram::setMatrix( const Maths::Matrix2& mat )
{
    glUniformMatrix2fv( _matUniform, 1, GL_FALSE, &mat[0] );
}

void Generic2DProgram::draw()
{
    _array.bind( _vertexPos );

    glDrawElements( GL_TRIANGLES, _array.count(), GL_UNSIGNED_SHORT, nullptr );
}