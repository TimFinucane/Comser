#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <sigc++/sigc++.h>
#pragma warning( push )
#pragma warning( disable : 4634 )
#pragma warning( disable : 4635 )
#include <SDL.h>
#pragma warning( pop )

#include <Rect.h>

// TODO: More options on mouse and keyboard input
class Window
{
public:
    typedef Maths::Rect<short> Rect;

    enum class Mode
    {
        WINDOWED,
        WINDOWED_BORDERLESS,
        FULLSCREEN
    };
    struct WindowSettings
    {
        std::string     name;
        Mode            mode;
        unsigned int    screen;

        // TODO: Options for middle of screen, bottom, etc
        Rect            rect;
    };
    struct ScreenSettings
    {
        Rect            rect;

        short           hDpi; // DPI vertical and horizontal
        short           vDpi;
    };

    typedef sigc::slot0<void>                           LoopFunction;
    typedef sigc::slot1<bool, SDL_Event>                EventFunction;
    typedef sigc::slot1<void, const WindowSettings&>    SettingsSlot;
    typedef sigc::signal1<void, const WindowSettings&>  SettingsSignal;
public:
    Window( const WindowSettings& settings );
    ~Window();

    void                    settings( const WindowSettings& settings );
    const WindowSettings&   settings() const
    {
        return _curSettings;
    }

    /// <summary>
    /// Lets the user be updated when window settings change
    /// </summary>
    sigc::connection        connectSettingsChange( SettingsSlot slot );

    /// <summary>
    /// Starts the main game loop. Function will send every event to the event function,
    ///  and when not processing events will call the loop function. The loop will quit when
    ///  the event function returns false.
    /// </summary>
    /// <param name="loop">The function containing your main game code</param>
    /// <param name="event">The function that will process all your events</param>
    void                    loop( LoopFunction loop, EventFunction event );

    /// <summary>
    /// Shows whatever was drawn on the screen
    ///  by swapping the opengl buffers
    /// </summary>
    void                    update();
    /// <summary>
    /// Clears the current buffer
    ///  and sets it to the given colour
    /// </summary>
    void                    clear( double r, double g, double b, double a );

    // Some static functions for getting info about the system.
    static unsigned int     defaultScreen();
    static unsigned int     numScreens();
    static ScreenSettings   getScreenInfo( unsigned int screen );
    // Produces a rect of given width and height that's centred on the given screens centre
    static Rect             getCentreRect( unsigned int screen, unsigned short width, unsigned short height );

    operator SDL_Window*()
    {
        return _window;
    }
private:
    SettingsSignal  _settingsSignal;
    WindowSettings  _curSettings;

    SDL_Window*     _window;
    void*           _context; // The opengl context
};

#endif