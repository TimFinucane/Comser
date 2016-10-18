#include "Shader.h"

#include <GraphicsIncludes.h>

using namespace Graphics::Shaders;

Shader::Shader( unsigned int shaderType, FileSystem::File& file )
{
    _shader = glCreateShader( shaderType );

    glShaderSource( _shader, 1, &file, &length );
    glCompileShader( _shader );

    int compiled = 0;
    glGetShaderiv( _shader, GL_COMPILE_STATUS, &compiled );
    if( compiled == GL_FALSE )
    {
        int len = 0;
        glGetShaderiv( _shader, GL_INFO_LOG_LENGTH, &len );

        std::string error( len, 0 );
        glGetShaderInfoLog( _shader, len, &len, &error[0] );

        glDeleteShader( _shader );

        throw std::runtime_error( "Error compiling shader: " + error );
    }

}
Shader::~Shader()
{
    glDeleteShader( _shader );
}