#include "Game.h"
#include "System.h"

#include <algorithm>

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

void Game::addSystem( System* system, UpdateOrder order )
{
    system->counterConnection() = _counter.signal( order, sigc::mem_fun( *system, &System::update ) );
    _systems.push_back( system );

    system->added( _scenes.begin(), _scenes.end() );
}
void Game::removeSystem( System* system )
{
    auto it = std::find_if( _systems.begin(), _systems.end(), [system]( const Systems::value_type& type ) { return type.first == system; } );

    if( it != _systems.end() )
        _systems.erase( it );
}

double Game::update()
{
    uint64_t time = SDL_GetPerformanceCounter();
    double ticks = ((time - _prevTime) / (double)_timerFreq) / _tickRate;
    _prevTime = time;

    _counter.update( ticks );

    return ticks;
}