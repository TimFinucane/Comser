#include "Device.h"

#include <GraphicsIncludes.h>
#include <SDL.h>
#include <Window.h>

using namespace Graphics;

void    Device::init( Window* window )
{
    glewInit();

    _context = SDL_GL_CreateContext( (*window) );
    
    _window = window;

    // Now the OpenGL settings
    // Alpha blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
}
void    Device::release()
{
    glDisable( GL_BLEND );
    glDisable( GL_CULL_FACE );

    SDL_GL_DeleteContext( _context );
}

void    Device::swap()
{
    SDL_GL_SwapWindow( *_window );
}
void    Device::clear()
{
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}