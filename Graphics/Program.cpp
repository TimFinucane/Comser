#include "Program.h"

#include <GraphicsIncludes.h>

using namespace Graphics::Shaders;

const unsigned int INVALID_PROGRAM = 0xDEADBEEF;

Program::Program()
{
    _program = glCreateProgram();
}

void    Program::create( const Shader* shaders, unsigned int count, std::string& error )
{
    // Attach shaders
    for( unsigned int i = 0; i < count; ++i )
    {
        glAttachShader( _program, shaders[i]._shader );
    }

    glLinkProgram( _program );

    // Check error status
    int linked = 0;
    glGetProgramiv( _program, GL_LINK_STATUS, &linked );

    if( linked == GL_FALSE )
    {
        int length = 0;
        glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &length );

        error.resize( length );
        glGetProgramInfoLog( _program, length, &length, &error[0] );
    }
}
void    Program::destroy()
{
    glDeleteProgram( _program );
}

void    Program::use()
{
    glUseProgram( _program );
}