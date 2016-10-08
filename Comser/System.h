#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include "Game.h"

namespace Comser
{
    class Game;
    // Purpose: A system is a process or series of processes that can hook to the engine for updating.

    // TODO: Docs
    // NB: To update, you simply hook an update function to the engine with the desired priority
    // Also can hook to an entity with desired node being enabled or disabled
    class System abstract
    {
    public:
        /// <summary>
        /// Creates the system, given the game to use the system on, and
        ///  the update category into which the
        System( Game* game, Comser::UpdateOrder order )
            : _game( game )
        {
            _counterCon = _game->_counter.signal( order, sigc::mem_fun( this, System::update ) );
        }
        ~System()
        {
            _counterCon.disconnect();
        }

        virtual void    sceneAdded( Game::SceneIterator scene ){}
        virtual void    sceneRemoved( Game::SceneIterator scene ){}
        virtual void    sceneDisabled( Game::SceneIterator scene ){}
        virtual void    sceneEnabled( Game::SceneIterator scene ){}
        
        virtual void    update(){}

        template<class... COMPONENTS>
        TupleHook<COMPONENTS...>   createTupleHook()
        {
            return TupleHook<COMPONENTS...>( *_game );
        }

        Game&     game()
        {
            return *_game;
        }
    private:
        Game*                           _game;
        sigc::connection                _counterCon;
    };
}

#endif