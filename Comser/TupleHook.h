#pragma once
#ifndef TUPLE_HOOK_H
#define TUPLE_HOOK_H

#include <tuple>
#include <map>
#include <array>
#include <sigc++/sigc++.h>

#include "Component.h"
#include "Entity.h"

#include "Game.h"

namespace Comser
{
    // Purpose: Used by a system to keep a reference to all
    //  entities which contain the desired components (tuple).
    // For sake of efficiency, put components in order of least->most used.
    template<class... COMPONENTS>
    class TupleHook
    {
    public:
        typedef std::tuple<COMPONENTS*...>                                  Tuple;

        typedef std::map<EntityId,Tuple>                                        Map;
        typedef typename Map::iterator                                          Iterator;
        typedef typename Map::const_iterator                                    ConstIterator;

        typedef sigc::signal<void, Scene::Group*, typename Map::value_type&>    Signal;

    private:
        static const unsigned __int32  COMPONENT_COUNT  = sizeof...(COMPONENTS);

        typedef std::array<Scene::LocalComponentType, COMPONENT_COUNT>  LocalTypes;

        class SceneTuples
        {
        public:
            SceneTuples( Scene::Group* gr, Signal& added, Signal& removed )
                : _group( gr ), _addedSig( added ), _removeSig( removed )
            {
                _localTypes[i] = LocalTypes{ group->localTypes( COMPONENTS::id() )... };

                for( int i = 0; i < _localTypes.size(); ++i )
                {
                    _group->connectAdded( _localTypes[i] ).connect( cinder::signals::slot( this, &TupleHook<COMPONENTS...>::_componentAdded ) );
                    _group->connectRemoved( _localTypes[i] ).connect( cinder::signals::slot( this, &TupleHook<COMPONENTS...>::_componentRemoved ) );
                }

                _addCreatedComponents();
            }

            ConstIterator       begin() const
            {
                return _map.begin();
            }
            ConstIterator       end() const
            {
                return _map.end();
            }

            /// <summary>
            /// Gets called when a component is added to an entity in this sceneset's scene.
            /// </summary>
            void                componentAdded( Scene::ComponentVector::ComponentInfo* componentInfo )
            {
                EntityId ent = componentInfo->entity;
                Tuple tuple;

                // Check to see if all the types are there
                if( _recurseCheck( ent, tuple ) )
                {
                    auto it = _map.emplace( ent, tuple );
                    addedSig.emit( _group, *it );
                }
            }
            /// <summary>
            /// Gets called when a component is removed from an entity in this sceneset's scene.
            /// </summary>
            void                componentRemoved( Scene::ComponentVector::ComponentInfo* componentInfo )
            {
                auto it = _map.find( componentInfo->entity );
                if( it != _map.end() && it->second )
                {
                    removeSig.emit( _group, *it );
                    map.erase( it );
                }
            }
        private:
            // Returns whether or not all components are present
            template <int STEPS = 0>
            bool         _recurseCheck( EntityId id, Tuple& tuple ) const
            {
                std::get<STEPS>( tuple ) = reinterpret_cast<std::tuple_element<STEPS, Tuple>::type>(group->getComponent( id, localTypes[STEPS] ));
                return (std::get<STEPS>( tuple ) != nullptr) && _recurseCheck<STEPS + 1>( group, id, tuple, localTypes );
            }
            template <>
            bool         _recurseCheck<COMPONENT_COUNT>( EntityId id, Tuple& tuple ) const
            {
                (group);
                (id);
                (tuple);
                (types);
                return true;
            }

            // Purpose: To add to the map components which were created before this TupleHook
            void                _addCreatedComponents()
            {
                for( ComponentVector::ConstIterator i = _group->begin( _localTypes[0] ); i != _group->end( _localtypes[0] ); ++i )
                {
                    // Now check if each of these components has all other components
                    Tuple tuple;
                    // Running recurse check for all but the first component
                    if( _recurseCheck( _group, tuple, &types[1] ) )
                    {
                        // Put in that first component
                        std::get<0>( tuple ) = reinterpret_cast<COMPONENTS>(i->component);

                        // Add it to our map
                        _tuples.emplace( i->entity, tuple );
                        tupleAdded.emit( _tuples.back() );
                    }
                }
            }

            Scene::Group*   _group;
            LocalTypes      _localTypes;
            Map             _map;

            Signal&         _addedSig;
            Signal&         _removeSig;
        };
    public:
        // TODO: Find all entities already created with those props
        void        sceneAdded( Game::SceneIterator* group )
        {
            sceneSets.emplace_back( group, tupleAdded, tupleRemoved );
        }
        void        sceneRemoved( Game::SceneIterator* group )
        {
            sceneSets.erase( std::find( sceneSets.begin(), sceneSets.end(), []( const SceneTupleSet& set ){ return set.group == group; } ) );
        }

        Signal      tupleAdded;
        Signal      tupleRemoved;
    private:
        TupleHook( Game& game )
        {
            for( auto i = game.scenesBegin(); i != game.scenesEnd(); ++i )
                sceneSets.emplace_back( &*i, tupleAdded, tupleRemoved );
        }

        std::vector<SceneTuples>    sceneTupleSet;
    };
}
#endif