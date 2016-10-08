#include "Window.h"

#include "SDLDev.h"

#include <sigc++/sigc++.h>
#include <tchar.h>

Window::Window()
{
    SDLDevice::create();
    int error = SDL_VideoInit( nullptr );
    sdlError( error );
}
Window::~Window()
{
}

void                            Window::init( const WindowSettings& settings )
{
    _curSettings = settings;

    /* Request opengl 3.2 context.
    *  SDL doesn't have the ability to choose which profile at this time of writing,
    *  but it should default to the core profile */
    // TODO: Choose a graphics device?
    int errorMaj = SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    int errorMin = SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    sdlError( errorMaj + errorMin, "Cannot setup OpenGL context of 3.2 or above. Please confirm you have drivers capable of supporting at least GL 3.2" );

    // TODO: Check opengl version

    // We want double buffering
    int error = SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    sdlError( error, "Double Buffering not enabled on this device" ); // TODO: Try get around this?

    int windowFlags = 0;
    windowFlags |= SDL_WINDOW_OPENGL;
    windowFlags |= SDL_WINDOW_SHOWN;

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

    // Do that cool thing where the game doesn't minimize if you open something else.
    SDL_SetHint( SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0" );
}
void                            Window::release()
{
    SDL_DestroyWindow( _window );
}

void                            Window::settings( const WindowSettings& settings )
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

                int error = SDL_GetDisplayBounds( SDL_GetWindowDisplayIndex( _window ), &rect );
                sdlError( error );

                SDL_SetWindowBordered( _window, SDL_FALSE );
                SDL_SetWindowSize( _window, rect.w, rect.h );
                break;
            }
        }
    }

    _settingsSignal( settings );
}

sigc::connection                Window::connectSettingsChange( SettingsSlot slot )
{
    return _settingsSignal.connect( slot );
}

void                            Window::loop( LoopFunction loop, EventFunction event )
{
    while( 1 )
    {
        SDL_Event e;
        while( SDL_PollEvent( &e ) )
        {
            if( !event( e ) ) return;
        }
        loop();
    }
}

unsigned int                    Window::defaultScreen()
{
    return 0; // TODO: Could it be anything other than this?
}
unsigned int                    Window::numScreens()
{
    int displays = SDL_GetNumVideoDisplays();
    sdlError( displays );

    return displays;
}
Window::ScreenSettings          Window::getScreenInfo( unsigned int screen )
{
    Window::ScreenSettings settings;

    float w = 0.0f;
    float h = 0.0f;
    int error = SDL_GetDisplayDPI( screen, nullptr, &w, &h );
    sdlError( error );

    settings.hDpi = (unsigned short)w;
    settings.vDpi = (unsigned short)h;

    SDL_Rect rect;
    error = SDL_GetDisplayBounds( screen, &rect );
    sdlError( error );

    settings.rect.x         = (short)rect.x;
    settings.rect.y         = (short)rect.y;
    settings.rect.width     = (short)rect.w;
    settings.rect.height    = (short)rect.h;

    return settings;
}

Window::Rect                    Window::getCentreRect( unsigned int screen, unsigned short width, unsigned short height )
{
    SDL_Rect sdlrect;
    int error = SDL_GetDisplayBounds( screen, &sdlrect );
    sdlError( error );

    int centrex = sdlrect.x + (sdlrect.w / 2);
    int centrey = sdlrect.y + (sdlrect.h / 2);

    Rect rect;
    rect.x = (unsigned short)centrex - width / 2;
    rect.width = width;
    rect.y = (unsigned short)centrey - height / 2;
    rect.height = height;

    return rect;
}