#include <CppUnitTest.h>

#include <Window.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

bool escapeFunc( SDL_Event e )
{
    // Not even trying to optomise, dw.
    if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE || 
        e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE )
        return false;
    return true;
}

TEST_CLASS( PlatformTests )
{
public:
    TEST_METHOD( simpleWindow )
    {
        try
        {
            sigc::signal0<void> signal;

            Window::WindowSettings settings;
            settings.name = "Window 1 - Windowed 800x600";
            settings.mode = Window::Mode::WINDOWED;
            settings.rect = Window::Rect{ 100, 100, 800, 600 };
            settings.screen = 0;

            Window simple;
            simple.init( settings );

            simple.loop( []{}, sigc::ptr_fun( &escapeFunc ) );

            simple.release();
        }
        catch( std::exception e )
        {
            Logger::WriteMessage( "Uncaught Exception in Simple Window:" );
            Logger::WriteMessage( e.what() );
        }
    }

    TEST_METHOD( centredWindow )
    {
        try
        {
            Window::WindowSettings settings;
            settings.name = "Window 2 - Centred 800x600";
            settings.mode = Window::Mode::WINDOWED;
            settings.rect = Window::getCentreRect( Window::defaultScreen(), 800, 600 );
            settings.screen = 0;

            Window window;
            window.init( settings );

            window.loop( [] {}, sigc::ptr_fun( &escapeFunc ) );

            window.release();
        }
        catch( std::exception e )
        {
            Logger::WriteMessage( "Uncaught Exception in Centred Window:" );
            Logger::WriteMessage( e.what() );
        }
    }

    TEST_METHOD( borderlessFullscreenWindow )
    {
        try
        {
            unsigned int screen = Window::defaultScreen();

            Window::WindowSettings settings;
            settings.name = "Window 3 - Borderless";
            settings.mode = Window::Mode::WINDOWED_BORDERLESS;
            settings.rect = Window::getScreenInfo( screen ).rect;
            settings.screen = screen;

            Window window;
            window.init( settings );

            window.loop( [] {}, sigc::ptr_fun( &escapeFunc ) );

            window.release();
        }
        catch( std::exception e )
        {
            Logger::WriteMessage( "Uncaught Exception in Borderless Fullscreen Window:" );
            Logger::WriteMessage( e.what() );
        }
    }

    TEST_METHOD( fullscreenWindowNormal )
    {
        try
        {
            unsigned int screen = Window::defaultScreen();

            Window::WindowSettings settings;
            settings.name = "Window 3 - Borderless";
            settings.mode = Window::Mode::FULLSCREEN;
            settings.rect = Window::getScreenInfo( screen ).rect;
            settings.screen = screen;

            Window window;
            window.init( settings );

            window.loop( [] {}, sigc::ptr_fun( &escapeFunc ) );

            window.release();
        }
        catch( std::exception e )
        {
            Logger::WriteMessage( "Uncaught Exception in Fullscreen Window Normal:" );
            Logger::WriteMessage( e.what() );
        }
    }
    TEST_METHOD( fullscreenWindowNoRect )
    {
        try
        {
            unsigned int screen = Window::defaultScreen();

            Window::WindowSettings settings;
            settings.name = "Window 3 - Borderless";
            settings.mode = Window::Mode::FULLSCREEN;
            settings.rect = { 0, 0, 0, 0 };
            settings.screen = screen;

            Window window;
            window.init( settings );

            window.loop( [] {}, sigc::ptr_fun( &escapeFunc ) );

            window.release();
        }
        catch( std::exception e )
        {
            Logger::WriteMessage( "Uncaught Exception in Fullscreen Window No Rect:" );
            Logger::WriteMessage( e.what() );
        }
    }
};