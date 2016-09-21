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

    void                init( const WindowSettings& settings );
    void                release();

    // Get info about the screen that the window is currently on
    ScreenSettings              getScreenInfo() const;

    void                        settings( const WindowSettings& settings );
    const WindowSettings&       settings()
    {
        return _curSettings;
    }

    sigc::connection    connectSettingsChange( SettingsSlot slot );

    void                loop( LoopFunction loop, EventFunction event );

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