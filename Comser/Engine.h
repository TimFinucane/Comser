#pragma once

#include <list>
#include <type_traits>
#include <memory>

#include "Scene.h"
#include "UpdateCounter.h"
#include "EventManager.h"

namespace Comser
{
    class System;

    class Engine
    {
    public:
        typedef std::list<std::unique_ptr<SceneBase>>               SceneList;
        typedef SceneList::iterator                                 SceneIterator;
        typedef std::vector<System*>   Systems;
    public:
        /// <summary>
        /// Initialises the game
        /// </summary>
        /// <param name="tickRate">The number of seconds per each game tick</param>
        /// <param name="orders">The number of categories your systems will fall into</param>
        Engine( double tickRate, unsigned int orders );
        /// <summary>
        /// Initialises the game
        /// </summary>
        /// <param name="tickRate">The number of seconds per each game tick</param>
        /// <param name="orderUpdateRates">A list of each system update category and the number of ticks per update for that category</param>
        Engine( double tickRate, std::initializer_list<unsigned int> orderUpdateRates );

        // <summary>
        // Creates a new scene, and returns the scene's id.
        // Starts disabled by default.
        // </summary>
        template <class SceneClass, typename... ARGS>
        SceneIterator       createScene( const std::initializer_list<ComponentType>& types, ARGS... args )
        {
            static_assert( std::is_base_of<SceneBase, SceneClass>::value, "error: created scene must be derived from Comser::Scene" );
            _scenes.emplace_front( std::make_unique<SceneClass>( types, args... ) );
            
            // TODO: Inform systems
            return _scenes.begin();
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
        void                setScene( SceneBase* scene, bool enable );

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

        Event::Manager      eventManager;
        UpdateCounter       counter;
        double              tickRate;
    private:
        SceneList           _scenes;

        Systems             _systems;

        uint64_t            _timerFreq;
        uint64_t            _prevTime;
    };
}