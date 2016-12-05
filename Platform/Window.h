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

// TODO: More options on mouse and keyboard input
class Window
{
public:
    struct Rect
    {
        int x, y, width, height;
    };

    enum class Mode
    {
        WINDOWED,
        WINDOWED_BORDERLESS,
        FULLSCREEN
    };
    struct ScreenSettings
    {
        Rect            rect;

        short           hDpi; // DPI vertical and horizontal
        short           vDpi;
    };

    typedef sigc::slot0<void>                           LoopFunction;
    typedef sigc::slot1<bool, SDL_Event>                EventFunction;
    typedef sigc::signal1<void, const Window&>          SettingsSignal;
public:
    Window( std::string name = "Window", Mode mode = Mode::WINDOWED, const Rect& rect = { 0, 0, 800, 600 } );
    ~Window();

    // Used for named parameter initialisation
    Window&         name( std::string name );
    Window&         mode( Mode mode );
    Window&         rect( const Rect& rect );

    std::string     name() const
    {
        return _name;
    }
    Mode            mode() const
    {
        return _mode;
    }
    const Rect&     rect() const
    {
        return _rect;
    }

    /// <summary>
    /// Lets the user be updated when window settings change
    /// </summary>
    sigc::connection        connectSettingsChange( SettingsSignal::slot_type slot );

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
    void                    clear( float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f );

    /// <summary>
    /// Updates then clears the window
    /// </summary>
    void                    flush( float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f )
    {
        update();
        clear( r, g, b, a );
    }

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

    std::string     _name;
    Mode            _mode;

    // TODO: Options for middle of screen, bottom, etc
    Rect            _rect;

    SDL_Window*     _window;
    void*           _context; // The opengl context
};

#endif