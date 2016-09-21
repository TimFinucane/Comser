#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Window.h>

class Window;

namespace Graphics
{
    class Device
    {
    public:
        void    init( Window* window );
        void    release();

        // Updates the screen by swapping the OpenGL buffers
        void    swap();

        void    clear();
    private:
        Window*         _window;
        void*           _context;
    };
}

#endif