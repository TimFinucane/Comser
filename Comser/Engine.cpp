#include "Engine.h"
#include "System.h"

#include <algorithm>

#include <SDL_timer.h>

using namespace Comser;

Engine::Engine( double tickRate, unsigned int orders )
    : _tickRate( tickRate )
{
    _counter.init( orders );

    _timerFreq = SDL_GetPerformanceFrequency();
    _prevTime = SDL_GetPerformanceCounter();
}
Engine::Engine( double tickRate, std::initializer_list<unsigned int> orderUpdateRates )
    : _tickRate( tickRate )
{
    _counter.init( orderUpdateRates );

    _timerFreq = SDL_GetPerformanceFrequency();
    _prevTime = SDL_GetPerformanceCounter();
}

void Engine::addSystem( System* system, UpdateOrder order )
{
    system->counterConnection() = _counter.signal( order, sigc::mem_fun( *system, &System::update ) );
    _systems.push_back( system );

    system->added( _scenes.begin(), _scenes.end() );
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
    double ticks = ((time - _prevTime) / (double)_timerFreq) / _tickRate;
    _prevTime = time;

    _counter.update( ticks );

    return ticks;
}