#pragma once

#include "Scene.h"

namespace Comser
{
    // A class that tracks a single instance of an entity
    // with the given components, in a scene that uses ENTITYREF.
    // Assumes that there is only ever one entity with all components.
    // Will work if entity is destroyed, then new entity is created, or if
    //  new entity is created then old entity is destroyed.
    template <typename ENTITYREF, typename... COMPONENTS>
    class SingleTracker
    {
    public:
        typedef Scene<ENTITYREF>            SceneType;
        using Ent =                         SceneType::CONSTREF;
        typedef std::tuple<COMPONENTS*...>  Tuple;
    public:
        SingleTracker( SceneType* scene )
            : _scene( scene ), _exists( false )
        {
            subscribe( sigc::mem_fun( *this, SingleTracker<ENTITYREF, COMPONENTS...>::componentAdded ),
                       sigc::mem_fun( *this, SingleTracker<ENTITYREF, COMPONENTS...>::componentRemoved ) );

            // TODO: search through the scene
        }
        SingleTracker( SceneType* scene, Ent item )
            : _scene( scene ), _entity( item ), _exists( true )
        {
            subscribe( sigc::mem_fun( *this, SingleTracker<ENTITYREF, COMPONENTS...>::componentAdded ),
                sigc::mem_fun( *this, SingleTracker<ENTITYREF, COMPONENTS...>::componentRemoved ) );


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
            return _tuple;
        }

    protected:
        void    componentAdded( Ent ent, Component* comp )
        {
            Tuple tuple;
            if( isItem( ent, tuple ) )
            {
                _exists = true;
                _entity = ent;
                _component = tuple;
            }
        }
        void    componentRemoved( Ent ent, Component* comp )
        {
            if( ent == _entity )
            {
                _exists = false;
            }
        }

        template <unsigned int N = 0>
        bool    isItem( Ent ent, Tuple& tuple )
        {
            return (std::get<N>( tuple ) = _scene->getComponent( ent, _scene->localType( std::tuple_element<N, Tuple>::type::id() ) ) != nullptr
                && isItem<N + 1>( ent, tuple );
        }
        template<>
        bool    isItem<sizeof...(COMPONENTS)>( Ent ent, Tuple& tuple )
        {
            return true;
        }

        template <unsigned int N = 0>
        void    subscribe( typename SceneType::Slot added, typename SceneType::Slot removed )
        {
            LocalComponentType localType = _scene->localType( std::tuple_element<N, Tuple>::type::id() );
            _added[N]   = _scene->connectAdded( localType, added );
            _removed[N] = _scene->connectRemoved( localType, removed );

            subscribe<N + 1>( slot );
        }
        template <>
        void    subscribe<sizeof...(COMPONENTS)>( typename SceneType::Slot, typename SceneType::Slot ) {}
    private:
        SceneType*          _scene;

        Ent                 _entity;
        Tuple               _components;
        bool                _exists;

        sigc::connection    _added[sizeof...(COMPONENTS)];
        sigc::connection    _removed[sizeof...(COMPONENTS)];
    };
}