#include "Window.h"

#include <SDL.h>

#include <sigc++/sigc++.h>
#include <tchar.h>

Window::Window()
{
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
}
Window::~Window()
{
    SDL_Quit();
}

void                    Window::init( const WindowSettings& settings )
{
    _curSettings = settings;

    /* Request opengl 3.2 context.
    * SDL doesn't have the ability to choose which profile at this time of writing,
    * but it should default to the core profile */
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

    // We want double buffering
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    // And an alpha level of around 2 bits, or 4 options
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 2 );

    int windowFlags = 0;
    windowFlags |= SDL_WINDOW_OPENGL;
    windowFlags |= SDL_WINDOW_SHOWN;
    windowFlags |= SDL_WINDOW_RESIZABLE;

    switch( settings.mode )
    {
    case WINDOWED:
        windowFlags |= SDL_WINDOW_RESIZABLE;
        break;
    case WINDOWED_BORDERLESS:
        windowFlags |= SDL_WINDOW_BORDERLESS;
        break;
    case FULLSCREEN:
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        break;
    }

    _window = SDL_CreateWindow( settings.name.c_str(), 
                                settings.rect.x, 
                                settings.rect.y, 
                                settings.rect.width, settings.rect.height, windowFlags );

    SDL_SetHint( SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0" );
}
void                    Window::release()
{
    SDL_DestroyWindow( _window );
}

void                    Window::settings( const WindowSettings& settings )
{
    // If the name changed, reset it
    if( settings.name != _curSettings.name )
        SDL_SetWindowTitle( _window, settings.name.c_str() );
    
    // If the position changed, reset it
    if( settings.rect.x == _curSettings.rect.x &&
        settings.rect.y == _curSettings.rect.y )
    {
        SDL_SetWindowPosition( _window, settings.rect.x, settings.rect.y );
    }

    // You get the idea
    if( settings.mode != _curSettings.mode )
    {
        switch( settings.mode )
        {
        case WINDOWED:
            SDL_SetWindowBordered( _window, SDL_TRUE );
            SDL_SetWindowSize( _window, settings.rect.width, settings.rect.height );
            break;
        case WINDOWED_BORDERLESS:
            SDL_SetWindowBordered( _window, SDL_FALSE );
            SDL_SetWindowSize( _window, settings.rect.width, settings.rect.height );
            break;
        case FULLSCREEN: // Note that we use fullscreen as windowed borderless because i do not like normal fullscreen
            {
                SDL_Rect rect;

                SDL_GetDisplayBounds( SDL_GetWindowDisplayIndex( _window ), &rect );

                SDL_SetWindowBordered( _window, SDL_FALSE );
                SDL_SetWindowSize( _window, rect.w, rect.h );
                break;
            }
        }
    }

    _settingsSignal( settings );
}

sigc::connection        Window::connectSettingsChange( SettingsSlot slot )
{
    return _settingsSignal.connect( slot );
}

void                    Window::loop( LoopFunction loop, EventFunction event )
{
    while( 1 )
    {
        SDL_Event e;
        while( SDL_PollEvent( &e ) )
        {
            if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE )
                return;
            else
                event( e );
        }
        loop();
    }
}

Window::ScreenSettings  Window::getScreenInfo() const
{
    Window::ScreenSettings settings;

    float w = 0.0f;
    float h = 0.0f;
    SDL_GetDisplayDPI( _curSettings.screen, nullptr, &w, &h );
    settings.hDpi = (unsigned short)w;
    settings.vDpi = (unsigned short)h;

    SDL_Rect rect;
    SDL_GetDisplayBounds( _curSettings.screen, &rect );

    settings.rect.x         = (short)rect.x;
    settings.rect.y         = (short)rect.y;
    settings.rect.width     = (short)rect.w;
    settings.rect.height    = (short)rect.h;

    return settings;
}