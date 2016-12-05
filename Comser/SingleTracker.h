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
            : SingleTracker::TrackerHelper( scene ), _exists( false )
        {
            subscribeAll();

            // TODO: search through the scene
        }
        SingleTracker( SceneType* scene, Ent item )
            : SingleTracker::TrackerHelper( scene ), _entity( item ), _exists( true )
        {
            subscribeAll();

            Tuple tuple;
            if( isItem( item, tuple ) )
                _components = tuple;
            else
                _exists = false;
        }
        SingleTracker()
            : _exists( false )
        {
        }

        ~SingleTracker()
        {
        }

        // Copy constructors
        SingleTracker( const SingleTracker& tracker )
        {
            operator =()( tracker );
        }
        SingleTracker& operator =( const SingleTracker& tracker )
        {
            scene = tracker.scene;

            // Automatically deletes previous subscriptions
            subscribeAll();

            if( tracker.exists() )
            {
                _exists = true;
                _entity = tracker._entity;
                _components = tracker._components;
            }
            else
                _exists = false;

            return *this;
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
        void    componentAdded( Ent ent, Component* )
        {
            Tuple tuple;
            if( isItem( ent, tuple ) )
            {
                _exists = true;
                _entity = ent;
                _components = tuple;
            }
        }
        void    componentRemoved( Ent ent, Component* )
        {
            if( ent == _entity )
            {
                _exists = false;
            }
        }

        void    subscribeAll()
        {
            subscribe( sigc::mem_fun( *this, &SingleTracker<ENTITYREF, COMPONENTS...>::componentAdded ),
                sigc::mem_fun( *this, &SingleTracker<ENTITYREF, COMPONENTS...>::componentRemoved ) );
        }
    private:
        Ent                 _entity;
        Tuple               _components;
        bool                _exists;
    };
}