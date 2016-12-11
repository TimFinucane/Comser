#pragma once

#include "Scene.h"

namespace Comser
{
    template <typename ENTITYREF, typename... COMPONENTS>
    struct TrackerHelper
    {
    public:
        typedef Scene<ENTITYREF>            SceneType;
        using Ent = typename SceneType::CONSTREF;
        typedef std::tuple<COMPONENTS*...>  Tuple;

    protected:
        TrackerHelper()
            : scene( nullptr )
        {
        }
        TrackerHelper( SceneType* scene )
            : scene( scene )
        {
            for( unsigned int i = 0; i < sizeof...(COMPONENTS); ++i )
            {
                _added[i].disconnect();
                _removed[i].disconnect();
            }
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
            _added[N] = scene->connectAdded( added, localType );
            _removed[N] = scene->connectRemoved( removed, localType );

            subscribe<N + 1>( added, removed );
        }
        template <>
        void    subscribe<sizeof...(COMPONENTS)>( typename SceneType::Signal::slot_type, typename SceneType::Signal::slot_type ) {}

        void disconnectAll()
        {
            for( unsigned int i = 0; i < sizeof...(COMPONENTS); ++i )
            {
                _added[i].disconnect();
                _removed[i].disconnect();
            }
        }

        SceneType* scene;

        sigc::connection    _added[sizeof...(COMPONENTS)];
        sigc::connection    _removed[sizeof...(COMPONENTS)];
    };
}