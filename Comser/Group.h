#pragma once
#ifndef ENTITY_GROUP_H
#define ENTITY_GROUP_H

#include <vector>
#include <sigc++/sigc++.h>

#include "Scene.h"

#include "GroupComponentManager.h"
#include "GroupEntityList.h"

namespace Comser
{
    namespace Group
    {
        // TODO: Decide whether we give the functionality for using LocalComponentType instead of ComponentType
  
        /// <summary>
        /// Holds all a group of entities and components.
        /// This functionality is useful for splitting up entities based on when/where they are used.
        /// </summary>
        class Scene final : public Comser::Scene
        {
            EntityList::EntityId getId( const EntityHandle& handle )
            {
                return *reinterpret_cast<EntityList::EntityId*>( handle.get() );
            }
        public:
            Scene( const std::initializer_list<ComponentType>& types );
            ~Scene();

            /// <summary>
            /// Creates an empty entity, with no components
            /// </summary>
            /// <returns>The id to that entity</returns>
            EntityHandle        createEntity();

            /// <summary>
            /// Destroys the entity, plus all it's components
            /// </summary>
            void                destroyEntity( EntityHandle handle );

            void                clear();


            // <summary>
            // Adds a single component to the given entity
            // </summary>
            // <typeparam name="COMPONENT">Class of item to add</typeparam>
            // <typeparam name="COMARGS">The types of the arguments for the COMPONENT's constructor</typeparam>
            // <param name="id">The entity Id</param>
            // <param name="localType">The local type of the component</param>
            // <param name="args">The args for the components constructor</param>
            template< class COMPONENT, typename... COMARGS >
            void                addComponent( EntityHandle handle, LocalComponentType localType, COMARGS... args )
            {
                ComponentVector::Index index = _components[localType]->push<COMPONENT, COMARGS...>( args... );

                _entities.addComponent( getId( handle ), localType, index );

                _addedSignals[localType.get()].emit( _components[localType]->get( index ) );
            }

            // <summary>
            // Adds a single component to the given entity. Slightly slower than the version with the LocalComponentType.
            // </summary>
            // <typeparam name="COMPONENT">Class of item to add</typeparam>
            // <typeparam name="COMARGS">The types of the arguments for the COMPONENT's constructor</typeparam>
            // <param name="id">The entity Id</param>
            // <param name="args">The args for the components constructor</param>
            template< class COMPONENT, typename... COMARGS >
            void                addComponent( EntityHandle handle, COMARGS... args )
            {
                LocalComponentType localType = _associator[COMPONENT::id()];

                ComponentVector::Index index = _components[localType]->push<COMPONENT, COMARGS...>( args... );

                _entities.addComponent( getId( handle ), localType, index );

                signalAdded( localType, handle, *_components[localType]->get( index ) );
            }

            template< class COMPONENT >
            void                removeComponent( EntityHandle handle )
            {
                EntityList::EntityIterator entityIt = _entities.findComponent( getId( handle ), _associator[COMPONENT::id] );

                _removeComponent<COMPONENT>( getId( handle ), entityIt );
            }
            void                removeComponent( EntityHandle handle, LocalComponentType localType )
            {
                EntityList::EntityIterator entityIt = _entities.findComponent( getId( handle ), localType );

                _removeComponent( getId( handle ), entityIt );
            }

            template< class COMPONENT >
            COMPONENT*          getComponent( EntityHandle handle )
            {
                LocalComponentType type = _associator[COMPONENT::id];
                EntityList::EntityIterator it = _entities.findComponent( getId( handle ), type );
                return reinterpret_cast<COMPONENT*>(*_components[type]->get( it->index ));
            }
            Component*          getComponent( EntityHandle handle, LocalComponentType localType )
            {
                EntityList::EntityIterator it = _entities.findComponent( getId( handle ), localType );
                // Tricky maths to avoid an extra if statement (cheeky)
                if( it == (*_entities[getId( handle )]).end() || it->type != localType )
                    return nullptr;

                return *_components[localType]->get( it->index );
            }

            // Entity vector Iterators
            const EntityList::ConstIterator         begin() const
            {
                return _entities.begin();
            }
            const EntityList::Iterator              begin()
            {
                return _entities.begin();
            }
            const EntityList::ConstIterator         end() const
            {
                return _entities.end();
            }
            const EntityList::Iterator              end()
            {
                return _entities.end();
            }

            EntityHandle                            getHandle( const EntityList::Iterator it )
            {
                return std::make_shared<EntityList::EntityId>( (unsigned int)std::distance( it, begin() ) );
            }

            // Component vector Iterators
            const ComponentVector::ConstIterator    begin( LocalComponentType type ) const
            {
                _components[type]->begin();
            }
            const ComponentVector::Iterator         begin( LocalComponentType type )
            {
                _components[type]->begin();
            }
            const ComponentVector::ConstIterator    end( LocalComponentType type ) const
            {
                _components[type]->end();
            }
            const ComponentVector::Iterator         end( LocalComponentType type )
            {
                _components[type]->end();
            }

            SceneType                               type()
            {
                return SceneType::GROUP;
            }
        private:
            /// <summary>
            /// Moves the component in the given entity iterator to the back of its component vector. 
            /// Used for pop and swap.
            /// NOTE: Not as fast as it used to be, but thats because before each component also had a reference to the entity that
            ///  owned it, and I think thats a bit too memory intensive.
            /// </summary>
            void                _swap( EntityList::EntityIterator entityIt );

            template< class COMPONENT >
            void                _removeComponent( EntityList::EntityId id, EntityList::EntityIterator entityIt )
            {
                // Call the signal
                signalRemoved( entityIt->type, std::make_shared<EntityList::EntityId>( id ), *_components[entityIt->type]->get( entityIt->index ) );

                // Move this component to last position in its component list
                _swap( entityIt->type, id, entityIt );

                // Pop it off the components list
                _components[entityIt->type]->pop<COMPONENT>();

                // And remove the item from the entity list
                _entities.removeComponent( id, entityIt );
            }
            void                _removeComponent( EntityList::EntityId id, EntityList::EntityIterator entityIt )
            {
                // Call the signal
                signalRemoved( entityIt->type, std::make_shared<EntityList::EntityId>( id ), *_components[entityIt->type]->get( entityIt->index ) );

                // Move this component to last position
                _swap( entityIt );

                // Pop it off the components
                _components[entityIt->type]->pop();

                // Remove the item from the entity
                _entities.removeComponent( id, entityIt );
            }

            Components          _components;
            EntityList          _entities;
        };
    }
}

#endif