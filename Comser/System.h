#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include "Engine.h"

namespace Comser
{
    // Purpose: A system is a process or series of processes that can hook to the engine for updating.

    // TODO: Docs
    // NB: To update, you simply hook an update function to the engine with the desired priority
    // Also can hook to an entity with desired node being enabled or disabled
    class System
    {
    public:
        sigc::connection&   counterConnection()
        {
            return _counterCon;
        }

        // Occurs on adding of the scene to a game
        virtual void        added( Engine*, const Engine::SceneIterator, const Engine::SceneIterator ){}

        virtual void        sceneCreated( Engine::SceneIterator scene ){}
        virtual void        sceneDestroyed( Engine::SceneIterator scene ){}
        virtual void        sceneDisabled( Engine::SceneIterator scene ){}
        virtual void        sceneEnabled( Engine::SceneIterator scene ){}
    private:
        sigc::connection    _counterCon;
    };
}

#endif