#include "Timer.h"

#include <SDL.h>

void    Timer::start()
{
    _lastTime = SDL_GetPerformanceCounter();
}
void    Timer::setTickRate( double tickperiod )
{
    Uint64 time;
    time = SDL_GetPerformanceFrequency();

    _tickrate = 1 / (time * tickperiod); // periods per tick
}
double  Timer::update()
{
    Uint64 cur;
    cur = SDL_GetPerformanceCounter();

    double frequency = double(cur - _lastTime);
    frequency *= _tickrate;

    _lastTime = cur;

    return frequency;
}