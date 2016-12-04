#pragma once

#include "Scene.h"

#include "TrackerHelper.h"

namespace Comser
{
    // A class that tracks a single instance of an entity
    // with the given components, in a scene that uses ENTITYREF.
    // Assumes that there is only ever one entity with all components.
    // Will work if entity is destroyed, then new entity is created, or if
    //  new entity is created then old entity is destroyed.
    template <typename ENTITYREF, typename... COMPONENTS>
    class SingleTracker : public TrackerHelper<ENTITYREF, COMPONENTS...>
    {
    public:
        SingleTracker( SceneType* scene )
            : scene( scene ), _exists( false )
        {
            subscribeAll();

            // TODO: search through the scene
        }
        SingleTracker( SceneType* scene, Ent item )
            : scene( scene ), _entity( item ), _exists( true )
        {
            subscribeAll();
        }
        SingleTracker()
            : scene( nullptr ), _exists( false )
        {
        }

        ~SingleTracker()
        {
        }

        bool    exists() const
        {
            return _exists;
        }
        Ent     entity() const
        {
            return _entity;
        }
        const Tuple& components() const
        {
            return _tuple;
        }
        Tuple&  components()
        {
            return _components;
        }

    protected:
        void    componentAdded( Ent ent, Component* comp )
        {
            Tuple tuple;
            if( isItem( ent, tuple ) )
            {
                _exists = true;
                _entity = ent;
                _components = tuple;
            }
        }
        void    componentRemoved( Ent ent, Component* comp )
        {
            if( ent == _entity )
            {
                _exists = false;
            }
        }

        void    subscribeAll()
        {
            subscribe( sigc::mem_fun( *this, SingleTracker<ENTITYREF, COMPONENTS...>::componentAdded ),
                sigc::mem_fun( *this, SingleTracker<ENTITYREF, COMPONENTS...>::componentRemoved ) );
        }
    private:
        Ent                 _entity;
        Tuple               _components;
        bool                _exists;
    };
}