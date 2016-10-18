#include "Program.h"

#include <GraphicsIncludes.h>

using namespace Graphics::Shaders;

const unsigned int INVALID_PROGRAM = 0xDEADBEEF;

Program::Program()
{
    _program = glCreateProgram();
}

std::string Program::create( std::initializer_list<const Shader*> shaders )
{
    // Attach shaders
    for( auto i = shaders.begin(); i != shaders.end(); ++i )
    {
        glAttachShader( _program, (*i)->_shader );
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