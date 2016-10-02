#include <CppUnitTest.h>

#include <Window.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS( PlatformTests )
{
public:
    TEST_METHOD( simpleWindow )
    {
        Window::WindowSettings settings;
        settings.name = "Window 1 - Windowed 800x600";
        settings.mode = Window::Mode::WINDOWED;
        settings.rect = Window::Rect{ 100, 100, 800, 600 };
        settings.screen = 0;

        Window simple;
        simple.init( settings );

        simple.loop( []{}, [](SDL_Event e){(e);} );

        simple.release();
    }

    TEST_METHOD( centredWindow )
    {
        Window::WindowSettings settings;
        settings.name = "Window 2 - Centred 800x600";
        settings.mode = Window::Mode::WINDOWED;
        settings.rect = Window::getCentreRect( Window::defaultScreen(), 800, 600 );
        settings.screen = 0;

        Window window;
        window.init( settings );

        window.loop( [] {}, []( SDL_Event e ) { (e); } );

        window.release();
    }

    TEST_METHOD( borderlessFullscreenWindow )
    {
        unsigned int screen = Window::defaultScreen();

        Window::WindowSettings settings;
        settings.name = "Window 3 - Borderless";
        settings.mode = Window::Mode::WINDOWED_BORDERLESS;
        settings.rect = Window::getScreenInfo( screen ).rect;
        settings.screen = screen;

        Window window;
        window.init( settings );

        window.loop( [] {}, []( SDL_Event e ) { (e); } );

        window.release();
    }

    TEST_METHOD( fullscreenWindowNormal )
    {
        unsigned int screen = Window::defaultScreen();

        Window::WindowSettings settings;
        settings.name = "Window 3 - Borderless";
        settings.mode = Window::Mode::FULLSCREEN;
        settings.rect = Window::getScreenInfo( screen ).rect;
        settings.screen = screen;

        Window window;
        window.init( settings );

        window.loop( [] {}, []( SDL_Event e ) { (e); } );

        window.release();
    }
    TEST_METHOD( fullscreenWindowNoRect )
    {
        unsigned int screen = Window::defaultScreen();

        Window::WindowSettings settings;
        settings.name = "Window 3 - Borderless";
        settings.mode = Window::Mode::FULLSCREEN;
        settings.rect = { 0, 0, 0, 0 };
        settings.screen = screen;

        Window window;
        window.init( settings );

        window.loop( [] {}, []( SDL_Event e ) { (e); } );

        window.release();
    }
};