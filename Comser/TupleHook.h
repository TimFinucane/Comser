#pragma once
#ifndef TUPLE_HOOK_H
#define TUPLE_HOOK_H

#include <tuple>
#include <map>
#include <array>
#include <sigc++/sigc++.h>

#include "Component.h"
#include "Entity.h"

#include "Scene.h"

namespace Comser
{
    // Purpose: Used by a system to keep a reference to all
    //  entities which contain the desired components (tuple).
    // For sake of efficiency, put components in order of least->most used.
    template<class... COMPONENTS>
    class TupleHook
    {
    public:
        typedef std::tuple<COMPONENTS*...>                                      Tuple;

        typedef std::map<EntityHandle,Tuple>                                    Map;
        typedef typename Map::iterator                                          Iterator;
        typedef typename Map::const_iterator                                    ConstIterator;

        typedef std::vector<SceneTuples>                                        SceneTupleSet;

        typedef sigc::signal<void, Scenes::Scene*, typename Map::value_type&>   Signal;

    private:
        static const unsigned __int32  COMPONENT_COUNT  = sizeof...(COMPONENTS);

        typedef std::array<Scene::LocalComponentType, COMPONENT_COUNT>  LocalTypes;

        class SceneTuples
        {
        public:
            SceneTuples( Scene* scene, Signal& added, Signal& removed )
                : _scene( scene ), _addedSig( added ), _removeSig( removed )
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
            /// Whether or not all components are present in the group
            /// </summary>
            static bool         componentsPresent()
            {
                for( unsigned int i = 0; i < _localTypes.size(); ++i )
                {
                    if( ComponentAssociator::NOT_FOUND( _localTypes[i] ) )
                        return false;
                }

                return true;
            }

            /// <summary>
            /// Gets called when a component is added to an entity in this sceneset's scene.
            /// </summary>
            void                componentAdded( EntityHandle id, Component* component )
            {
                (component);
                Tuple tuple;

                // Check to see if all the types are there
                if( _recurseCheck( id, tuple ) )
                {
                    auto it = _map.emplace( id, tuple );
                    addedSig.emit( _group, *it );
                }
            }
            /// <summary>
            /// Gets called when a component is removed from an entity in this sceneset's scene.
            /// </summary>
            void                componentRemoved( EntityHandle id, Component* component )
            {
                auto it = _map.find( componentInfo->entity );
                if( it != _map.end() && it->second )
                {
                    removeSig.emit( _group, *it );
                    map.erase( it );
                }
            }

            Scene*              scene();
        private:
            // Returns whether or not all components are present
            template <int STEPS = 0>
            bool         _recurseCheck( EntityHandle id, Tuple& tuple ) const
            {
                std::get<STEPS>( tuple ) = reinterpret_cast<std::tuple_element<STEPS, Tuple>::type>(group->getComponent( id, localTypes[STEPS] ));
                return (std::get<STEPS>( tuple ) != nullptr) && _recurseCheck<STEPS + 1>( group, id, tuple, localTypes );
            }
            template <>
            bool         _recurseCheck<COMPONENT_COUNT>( EntityHandle id, Tuple& tuple ) const
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

            Scene*          _scene;
            LocalTypes      _localTypes;
            Map             _map;

            Signal&         _addedSig;
            Signal&         _removeSig;
        };
    public:

        /// <summary>
        /// Gets the scene tuple containing all entities
        ///  with the desired components in the given scene.
        /// If this scene tuple is not present it will be made,
        ///  unless not all component types are present, in which
        ///  case it returns nullptr.
        /// </summary>
        SceneTuples*            getTuples( Scene* scene )
        {
            auto it = std::find( _sceneTupleSet.begin(), _sceneTupleSet.end(), [scene]( SceneTuples& it ){ return it.scene() == scene; } );
            if( it != _sceneTupleSet.end() )
                return it;

            if( !SceneTuples::componentsPresent( scene ) )
                return nullptr;

            _sceneTupleSet.emplace_back( scene, _added, _removed );
        }

        /// <summary>
        /// Destroys the given scene tuples.
        /// Does not actually destroy the entities, just the recording the
        ///  tuple hook has of them.
        /// </summary>
        SceneTupleSet::iterator destroyTuples( SceneTupleSet::iterator it )
        {
            return _sceneTupleSet.erase( it );
        }
        /// <summary>
        /// Destroys the given scene tuples.
        /// Does not actually destroy the entities, just the recording the
        ///  tuple hook has of them.
        /// </summary>
        void                    destroyTuples( Scene* scene )
        {
            auto it = std::find( _sceneTupleSet.begin(), _sceneTupleSet.end(), [scene]( SceneTuples& it ) { return it.scene() == scene; } );
            if( it == _sceneTupleSet.end() )
                return;

            destroyTuples( it );
        }

        /// <summary>
        /// Calls the slot whenever an entity with all requested
        ///  components is created.
        /// </summary>
        sigc::connection        connectAdded( Signal::slot_type slot )
        {
            return _added.connect( slot );
        }
        /// <summary>
        /// Calls the slot whenever an entity with all requested
        ///  components is destroyed.
        /// </summary>
        sigc::connection        connectRemoved( Signal::slot_type slot )
        {
            return _removed.connect( slot );
        }

        SceneTupleSet::iterator tuplesBegin()
        {
            return _sceneTupleSet.begin();
        }
        SceneTupleSet::iterator tuplesEnd()
        {
            return _sceneTupleSet.end();
        }
    private:
        Signal          _added;
        Signal          _removed;

        SceneTupleSet   _sceneTupleSet;
    };
}
#endif