#include "Game.h"

#include <SDL_timer.h>

using namespace Comser;

Game::Game( double tickRate, unsigned int orders )
    : _tickRate( tickRate )
{
    _counter.init( orders );

    _timerFreq = SDL_GetPerformanceFrequency();
    _prevTime = SDL_GetPerformanceCounter();
}
Game::Game( double tickRate, std::initializer_list<unsigned int> orderUpdateRates )
    : _tickRate( tickRate )
{
    _counter.init( orderUpdateRates );

    _timerFreq = SDL_GetPerformanceFrequency();
    _prevTime = SDL_GetPerformanceCounter();
}

double Game::update()
{
    uint64_t time = SDL_GetPerformanceCounter();
    double ticks = ((time - _prevTime) / (double)_timerFreq) / _tickRate;
    _prevTime = time;

    if( ticks < 20.0 )
        _counter.update( ticks );

    return ticks;
}