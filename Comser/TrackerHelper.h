#pragma once

#include "Scene.h"

namespace Comser
{
    template <typename ENTITYREF, typename... COMPONENTS>
    struct TrackerHelper
    {
    public:
        typedef Scene<ENTITYREF>                            SceneType;
        typedef typename SceneType::CONSTREF                Ent;

        typedef std::tuple<COMPONENTS*...>                  Tuple;
        typedef typename sigc::signal<void, Ent, Tuple*>    Signal;

        sigc::connection    connectAdd( typename Signal::slot_type slot )
        {
            return added.connect( slot );
        }
        sigc::connection    connectRemove( typename Signal::slot_type slot )
        {
            return removed.connect( slot );
        }

    protected:
        TrackerHelper()
            : scene( nullptr )
        {
        }
        TrackerHelper( SceneType* scene )
            : scene( scene )
        {
        }

        ~TrackerHelper()
        {
            disconnectAll();
        }

        TrackerHelper( const TrackerHelper& tracker ) = delete;
        TrackerHelper& operator =( const TrackerHelper& helper ) = delete;
        TrackerHelper( TrackerHelper&& tracker ) = delete;
        TrackerHelper&& operator =( TrackerHelper&& helper ) = delete;

        template <unsigned int N = 0>
        bool    isItem( Ent ent, Tuple& tuple )
        {
            typedef std::remove_pointer<std::tuple_element<N, Tuple>::type>::type CompType;

            std::get<N>( tuple ) = (CompType*)scene->getComponent( ent, scene->localType( CompType::id() ) );

            return std::get<N>( tuple ) != nullptr && isItem<N + 1>( ent, tuple );
        }
        template<>
        bool    isItem<sizeof...(COMPONENTS)>( Ent, Tuple& )
        {
            return true;
        }

        template <unsigned int N = 0>
        void    subscribe( typename SceneType::Signal::slot_type added, typename SceneType::Signal::slot_type removed )
        {
            typedef std::remove_pointer<std::tuple_element<N, Tuple>::type>::type CompType;

            LocalComponentType localType = scene->localType( CompType::id() );
            _compAdded[N] = scene->connectAdded( added, localType );
            _compRemoved[N] = scene->connectRemoved( removed, localType );

            subscribe<N + 1>( added, removed );
        }
        template <>
        void    subscribe<sizeof...(COMPONENTS)>( typename SceneType::Signal::slot_type, typename SceneType::Signal::slot_type ) {}

        void disconnectAll()
        {
            for( unsigned int i = 0; i < sizeof...(COMPONENTS); ++i )
            {
                _compAdded[i].disconnect();
                _compRemoved[i].disconnect();
            }
        }

        SceneType* scene;

        Signal      added;
        Signal      removed;

    private:

        sigc::connection    _compAdded[sizeof...(COMPONENTS)];
        sigc::connection    _compRemoved[sizeof...(COMPONENTS)];
    };
}