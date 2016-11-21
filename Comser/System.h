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
    class System
    {
    public:

        virtual void    sceneAdded( Game::SceneIterator scene ){}
        virtual void    sceneRemoved( Game::SceneIterator scene ){}
        virtual void    sceneDisabled( Game::SceneIterator scene ){}
        virtual void    sceneEnabled( Game::SceneIterator scene ){}
        
        virtual void    update(){}

        //virtual bool    

    private:
        sigc::connection                _counterCon;
    };
}

#endif