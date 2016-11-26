#include "Window.h"

#include "SDLDev.h"
#include "GraphicsIncludes.h"

#include <sigc++/sigc++.h>
#include <tchar.h>

Window::Window( std::string name, Mode mode, const Rect& rect, unsigned int screen )
{
    SDLDevice::create();
    // TODO: Option to change device
    int error = SDL_VideoInit( nullptr );
    sdlError( error );

    int windowFlags = 0;
    windowFlags |= SDL_WINDOW_OPENGL;
    windowFlags |= SDL_WINDOW_SHOWN;

    _window = SDL_CreateWindow( name.c_str(),
                                0, 0, 0, 0, 
                                windowFlags );

    // Do that cool thing where the game doesn't minimize if you open something else.
    SDL_SetHint( SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0" );

    /* Request opengl 3.2 context.
    *  SDL doesn't have the ability to choose which profile at this time of writing,
    *  but it should default to the core profile */
    // TODO: Choose a graphics device?
    int errorMaj = SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    int errorMin = SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    sdlError( errorMaj + errorMin, "Cannot setup OpenGL context of 3.2 or above. Please confirm you have drivers capable of supporting at least GL 3.2" );

    // TODO: Check opengl version

    // We want double buffering
    error = SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    sdlError( error, "Double Buffering not enabled on this device" ); // TODO: Try get around this?

    _context = SDL_GL_CreateContext( _window );

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
        throw std::runtime_error( "Error intializing opengl with glew: " + std::string( (char*)glewGetErrorString( glewError ) ) );

    _name = name;
    this->mode( mode ).screen( screen ).rect( rect );

}
Window::~Window()
{
    SDL_GL_DeleteContext( _context );
    SDL_DestroyWindow( _window );
}

Window&         Window::name( std::string name )
{
    _name = name;

    SDL_SetWindowTitle( _window, name.c_str() );

    _settingsSignal( *this );
    return *this;
}
Window&         Window::mode( Mode mode )
{
    switch( mode )
    {
    case Mode::WINDOWED:
        SDL_SetWindowBordered( _window, SDL_TRUE );
        break;
    case Mode::WINDOWED_BORDERLESS:
        SDL_SetWindowBordered( _window, SDL_FALSE );
        break;
    case Mode::FULLSCREEN:
    {
        // Use the windows resolution, so we dont get nasty low res stuff going on
        SDL_Rect rect;

        int error = SDL_GetDisplayBounds( SDL_GetWindowDisplayIndex( _window ), &rect );
        if( sdlError( error ) )
            return *this;

        SDL_SetWindowBordered( _window, SDL_FALSE );
        SDL_SetWindowSize( _window, rect.w, rect.h );
        break;
    }
    }

    _mode = mode;

    _settingsSignal( *this );

    return *this;
}
Window&         Window::rect( const Rect& rect )
{
    SDL_Rect display;
    int error = SDL_GetDisplayBounds( _screen, &display );
    if( sdlError( error ) )
        return *this;

    _rect = rect;

    _rect.x += display.x;
    _rect.y += display.y;

    SDL_SetWindowPosition( _window, _rect.x, _rect.y );
    SDL_SetWindowSize( _window, _rect.width, _rect.height );

    glViewport( 0, 0, rect.width, rect.height );

    _settingsSignal( *this );

    return *this;
}
Window&         Window::screen( unsigned int screen )
{
    if( screen < (unsigned int)SDL_GetNumVideoDisplays() )
    {
        _screen = screen;
        // Reposition window
        return rect( _rect );
    }
    else
        return *this;
}

sigc::connection                Window::connectSettingsChange( SettingsSignal::slot_type slot )
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

void                            Window::update()
{
    SDL_GL_SwapWindow( _window );
}
void                            Window::clear( float r, float g, float b, float a )
{
    glClearColor( r, g, b, a );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

unsigned int                    Window::defaultScreen()
{
    SDLDevice::create();

    return 0; // TODO: Could it be anything other than this?
}
unsigned int                    Window::numScreens()
{
    SDLDevice::create();

    int displays = SDL_GetNumVideoDisplays();
    sdlError( displays );

    return displays;
}
Window::ScreenSettings          Window::getScreenInfo( unsigned int screen )
{
    SDLDevice::create();

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
    SDLDevice::create();

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