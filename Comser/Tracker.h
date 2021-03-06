#pragma once

#pragma once

#include <tuple>
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
    class Tracker : public TrackerHelper<ENTITYREF, COMPONENTS...>
    {
    public:
        struct Item
        {
            Ent     ent;
            Tuple   tuple;

            bool operator ==( Ent entity )
            {
                return ent == entity;
            }
        };
        typedef std::vector<Item>   Vector;

    public:
        Tracker( SceneType* scene )
            : Tracker::TrackerHelper( scene )
        {
            subscribe( sigc::mem_fun( *this, &Tracker<ENTITYREF, COMPONENTS...>::componentAdded ),
                sigc::mem_fun( *this, &Tracker<ENTITYREF, COMPONENTS...>::componentRemoved ) );

            // TODO: search through the scene
        }
        Tracker()
        {
        }

        ~Tracker()
        {
        }

        Tracker( const Tracker& tracker )
        {
            _scene = tracker.scene;

            _items = tracker._items;

            subscribe( sigc::mem_fun( *this, &Tracker<ENTITYREF, COMPONENTS...>::componentAdded ),
                sigc::mem_fun( *this, &Tracker<ENTITYREF, COMPONENTS...>::componentRemoved ) );
        }
        Tracker& operator =( const Tracker& tracker )
        {
            scene = tracker.scene;

            _items = tracker._items;

            disconnectAll();
            subscribe( sigc::mem_fun( *this, &Tracker<ENTITYREF, COMPONENTS...>::componentAdded ),
                sigc::mem_fun( *this, &Tracker<ENTITYREF, COMPONENTS...>::componentRemoved ) );

            return *this;
        }

        typename Vector::const_iterator  begin() const
        {
            return _items.begin();
        }
        typename Vector::const_iterator  end() const
        {
            return _items.end();
        }

    protected:
        void    componentAdded( Ent ent, Component* )
        {
            Tuple tuple;
            if( isItem( ent, tuple ) )
            {
                _items.emplace_back( Item{ ent, tuple } );
                added( ent, &tuple );
            }
        }
        void    componentRemoved( Ent ent, Component* )
        {
            // TODO: faster?
            Vector::iterator it;
            if( (it = std::find( _items.begin(), _items.end(), ent )) != _items.end() )
            {
                removed( ent, &it->tuple );

                std::iter_swap( it, _items.end() - 1 );
                _items.pop_back();
            }
        }

    private:
        Vector              _items;
    };
}