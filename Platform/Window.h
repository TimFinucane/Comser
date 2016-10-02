#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <sigc++/sigc++.h>
#include <SDL.h>

#include <Rect.h>

// TODO: More options on mouse and keyboard input
class Window
{
public:
    typedef Maths::Rect<short> Rect;

    enum Mode
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
    typedef sigc::slot1<void, SDL_Event>                EventFunction;
    typedef sigc::slot1<void, const WindowSettings&>    SettingsSlot;
    typedef sigc::signal1<void, const WindowSettings&>  SettingsSignal;
public:
    Window();
    ~Window();

    void                    init( const WindowSettings& settings );
    void                    release();

    void                    settings( const WindowSettings& settings );
    const WindowSettings&   settings() const
    {
        return _curSettings;
    }

    // Lets you get updated when screen settings are changed by the program
    //  through the settings method
    sigc::connection        connectSettingsChange( SettingsSlot slot );

    void                    loop( LoopFunction loop, EventFunction event );

    // Some static functions for getting info about the system
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
};

#endif