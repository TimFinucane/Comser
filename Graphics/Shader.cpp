#include "Shader.h"

#include <GraphicsIncludes.h>

using namespace Graphics::Shaders;

Shader::Shader( unsigned int shaderType )
{
    _shader = glCreateShader( shaderType );
}
Shader::~Shader()
{
    glDeleteShader( _shader );
}

bool Shader::load( const char* file, int length, std::string& error )
{
    glShaderSource( _shader, 1, &file, &length );
    glCompileShader( _shader );

    int compiled = 0;
    glGetShaderiv( _shader, GL_COMPILE_STATUS, &compiled );
    if( compiled == GL_FALSE )
    {
        int length = 0;
        glGetShaderiv( _shader, GL_INFO_LOG_LENGTH, &length );

        error.resize( length );
        glGetShaderInfoLog( _shader, length, &length, &error[0] );

        glDeleteShader( _shader );

        return false;
    }

    return true;
}