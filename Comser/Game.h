#pragma once

#include <list>
#include <type_traits>

#include "Scene.h"
#include "UpdateCounter.h"

namespace Comser
{
    class System;

    class Game
    {
    public:
        typedef std::list<Scene*>       SceneList;
        typedef SceneList::iterator     SceneIterator;
        typedef std::vector<System>     Systems;
    public:
        /// <summary>
        /// Initialises the game
        /// </summary>
        /// <param name="tickRate">The number of seconds per each game tick</param>
        /// <param name="orders">The number of categories your systems will fall into</param>
        Game( double tickRate, unsigned int orders );
        /// <summary>
        /// Initialises the game
        /// </summary>
        /// <param name="tickRate">The number of seconds per each game tick</param>
        /// <param name="orderUpdateRates">A list of each system update category and the number of ticks per update for that category</param>
        Game( double tickRate, std::initializer_list<unsigned int> orderUpdateRates );

        /// <summary>
        /// Will set a certain number of ticks per update for the given update order
        /// </summary>
        /// <param name="order">The order to set</param>
        /// <param name="ticksPerUpdate">The number of game ticks to wait before updating</param>
        void                counter( UpdateOrder order, unsigned int ticksPerUpdate )
        {
            _counter.ticksPerUpdate( order, ticksPerUpdate );
        }

        // <summary>
        // Creates a new scene, and returns the scene's id.
        // Starts disabled by default.
        // </summary>
        template <class SceneClass, typename... ARGS>
        SceneIterator       createScene( const std::initializer_list<ComponentType>& types, ARGS... args )
        {
            static_assert( std::is_base_of<Scene, SceneClass>::value, "error: created scene must be derived from Comser::Scene" );
            _scenes.emplace_back( new SceneClass( types, args... ) );
            
            // TODO: Inform systems

            return _scenes.end()--;
        }
        void                destroyScene( SceneIterator it )
        {
            // TODO: Warn systems
            _scenes.erase( it );
        }

        /// <summary>
        /// Will enable/disable the given scene, determining whether or not it is used in updates
        /// </summary>
        void                setScene( SceneIterator* scene, bool enable );
        void                setScene( Scene* scene, bool enable );

        void                addSystem( System* system );
        void                removeSystem( System* system );

        /// <summary>
        /// Updates all systems in the set
        /// </summary>
        /// <returns>Time passed since last update in ticks</returns>
        double              update();

        SceneIterator       scenesBegin()
        {
            return _scenes.begin();
        }
        SceneIterator       scenesEnd()
        {
            return _scenes.end();
        }

        // TODO: Scenes added, removed, enabled, and disabled
    private:
        SceneList                   _scenes;

        UpdateCounter               _counter;

        uint64_t                    _timerFreq;
        uint64_t                    _prevTime;

        double                      _tickRate;
    };
}