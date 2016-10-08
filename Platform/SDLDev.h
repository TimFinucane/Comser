#pragma once

#include <memory>
#include <SDL.h>

// The default method for dealing with SDL errors
inline bool sdlError( int error )
{
    if( error < 0 )
        throw std::runtime_error( SDL_GetError() );

    return (error < 0);
}
inline bool sdlError( int error, const char* message )
{
    if( error < 0 )
        throw std::runtime_error( message );

    return (error < 0);
}

// A class ensuring SDLInit and SDLQuit are only called once
class SDLDevice
{
public:
    static void create()
    {
        static SDLDevice dev;
    }

private:
    SDLDevice()
    {
        int error = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER );
        sdlError( error );
    }
    ~SDLDevice()
    {
        SDL_Quit();
    }
};