#include "Program.h"

#include <GraphicsIncludes.h>

using namespace Graphics::Shaders;

const unsigned int INVALID_PROGRAM = 0xDEADBEEF;

Program::Program( std::initializer_list<Shader> shaders )
{
    _program = glCreateProgram();

    // Attach shaders
    for( auto i = shaders.begin(); i != shaders.end(); ++i )
        glAttachShader( _program, i->_shader );

    glLinkProgram( _program );

    // Check error status
    int linked = 0;
    glGetProgramiv( _program, GL_LINK_STATUS, &linked );

    if( linked == GL_FALSE )
    {
        int length = 0;
        glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &length );

        std::string error( length, 0 );
        glGetProgramInfoLog( _program, length, &length, &error[0] );

        throw std::runtime_error( "Error linking program: " + error );
    }
}
Program::~Program()
{
    glDeleteProgram( _program );
}

void    Program::use()
{
    glUseProgram( _program );
}