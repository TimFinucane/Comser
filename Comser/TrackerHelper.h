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
        template <unsigned int N = 0>
        bool    isItem( Ent ent, Tuple& tuple )
        {
            typedef std::remove_pointer<std::tuple_element<N, Tuple>::type>::type CompType;

            std::get<N>( tuple ) = (CompType*)_scene->getComponent( ent, _scene->localType( CompType::id() ) );

            return std::get<N>( tuple ) != nullptr && isItem<N + 1>( ent, tuple );
        }
        template<>
        bool    isItem<sizeof...(COMPONENTS)>( Ent ent, Tuple& tuple )
        {
            return true;
        }

        template <unsigned int N = 0>
        void    subscribe( typename SceneType::Slot added, typename SceneType::Slot removed )
        {
            typedef std::remove_pointer<std::tuple_element<N, Tuple>::type>::type CompType;

            LocalComponentType localType = _scene->localType( CompType::id() );
            _added[N] = _scene->connectAdded( localType, added );
            _removed[N] = _scene->connectRemoved( localType, removed );

            subscribe<N + 1>( added, removed );
        }
        template <>
        void    subscribe<sizeof...(COMPONENTS)>( typename SceneType::Slot, typename SceneType::Slot ) {}

        SceneType* scene;

        sigc::connection    _added[sizeof...(COMPONENTS)];
        sigc::connection    _removed[sizeof...(COMPONENTS)];
    };
}