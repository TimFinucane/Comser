#include "Engine.h"
#include "System.h"

#include <algorithm>

#include <SDL_timer.h>

using namespace Comser;

Engine::Engine( double tickRate, unsigned int orders )
    : tickRate( tickRate ), counter( orders )
{
    _timerFreq = SDL_GetPerformanceFrequency();
    _prevTime = SDL_GetPerformanceCounter();
}
Engine::Engine( double tickRate, std::initializer_list<unsigned int> orderUpdateRates )
    : tickRate( tickRate ), counter( orderUpdateRates )
{
    _timerFreq = SDL_GetPerformanceFrequency();
    _prevTime = SDL_GetPerformanceCounter();
}

void Engine::addSystem( System* system )
{
    _systems.push_back( system );

    system->added( this, _scenes.begin(), _scenes.end() );
}
void Engine::removeSystem( System* system )
{
    auto it = std::find_if( _systems.begin(), _systems.end(), [system]( const System* a ) { return a == system; } );

    if( it != _systems.end() )
        _systems.erase( it );
}

double Engine::update()
{
    uint64_t time = SDL_GetPerformanceCounter();
    double ticks = ((time - _prevTime) / (double)_timerFreq) / tickRate;
    _prevTime = time;

    counter.update( ticks );

    return ticks;
}