#pragma once
#ifndef ENTITY_GROUP_H
#define ENTITY_GROUP_H

#include <vector>
#include <sigc++/sigc++.h>

#include "ComponentManager.h"
#include "EntityList.h"

namespace Comser
{
    namespace Scene
    {
        // TODO: Decide whether we give the functionality for using LocalComponentType instead of ComponentType
  
        /// <summary>
        /// Holds all a group of entities and components.
        /// This functionality is useful for splitting up entities based on when/where they are used.
        /// </summary>
        class Group
        {
        public:
            typedef sigc::signal<void, ComponentVector::ComponentInfo*>     Signal;
            typedef std::vector<Signal>                                     SignalList;

            friend class Game;
        public:
            Group( const std::initializer_list<ComponentType>& types );
            ~Group();

            /// <summary>
            /// Creates an empty entity, with no components
            /// </summary>
            /// <returns>The id to that entity</returns>
            EntityId            createEntity();

            /// <summary>
            /// Destroys the entity, plus all it's components
            /// </summary>
            void                destroyEntity( EntityId id );

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
            void                addComponent( EntityId id, LocalComponentType localType, COMARGS... args )
            {
                ComponentVector::Index index = _components[localType]->push<COMPONENT, COMARGS...>( id, args... );

                _entities.addComponent( id, localType, index );

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
            void                addComponent( EntityId id, COMARGS... args )
            {
                LocalComponentType localType = _associator[COMPONENT::id()];

                ComponentVector::Index index = _components[localType]->push<COMPONENT, COMARGS...>( id, args... );

                _entities.addComponent( id, localType, index );

                _addedSignals[localType.get()].emit( _components[localType]->get( index ) );
            }

            template< class COMPONENT >
            void                removeComponent( EntityId id )
            {
                EntityList::EntityIterator entityIt = _entities.findComponent( id, _associator[COMPONENT::id] );

                _removeComponent<COMPONENT>( id, entityIt );
            }
            void                removeComponent( EntityId id, LocalComponentType localType )
            {
                EntityList::EntityIterator entityIt = _entities.findComponent( id, localType );

                _removeComponent( id, entityIt );
            }

            template< class COMPONENT >
            COMPONENT*          getComponent( EntityId id )
            {
                LocalComponentType type = _associator[COMPONENT::id];
                EntityList::EntityIterator it = _entities.findComponent( id, type );
                return reinterpret_cast<COMPONENT*>(_components[type]->get( it->index )->component);
            }
            Component*          getComponent( EntityId id, LocalComponentType localType )
            {
                EntityList::EntityIterator it = _entities.findComponent( id, localType );
                // Tricky maths to avoid if statement (cheeky)
                if( it == (*_entities[id]).end() || it->type != localType )
                    return nullptr;

                return (_components[localType]->get( it->index )->component);
            }

            sigc::connection    connectAdded( LocalComponentType type, Signal::slot_type slot )
            {
                return _addedSignals[type.get()].connect( slot );
            }
            sigc::connection    connectRemoved( LocalComponentType type, Signal::slot_type slot )
            {
                return _removedSignals[type.get()].connect( slot );
            }

            LocalComponentType  operator[]( ComponentType type )
            {
                return _associator[type];
            }
            LocalComponentType  localType( ComponentType type )
            {
                return (*this)[type];
            }

            bool                active()
            {
                return _active;
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
        private:
            void                active( bool enable )
            {
                _active = enable;
            }

            // <summary>
            // Moves the component in the given entity iterator to the back of its component vector.
            // Useful for pop & swap.
            // </summary>
            void                _swap( EntityList::EntityIterator entityIt );

            bool                _active;

            template< class COMPONENT >
            void                _removeComponent( EntityId id, EntityList::EntityIterator entityIt )
            {
                // Call the signal
                _removedSignals[entityIt->type.get()].emit( _components[entityIt->type]->get( entityIt->index ) );

                // Move this component to last position in its component list
                _swap( entityIt->type, id, entityIt );

                // Pop it off the components list
                _components[entityIt->type]->pop<COMPONENT>();

                // And remove the item from the entity list
                _entities.removeComponent( id, entityIt );
            }
            void                _removeComponent( EntityId id, EntityList::EntityIterator entityIt )
            {
                // Call the signal
                _removedSignals[entityIt->type.get()].emit( &*_components[entityIt->type]->get( entityIt->index ) );

                // Move this component to last position
                _swap( entityIt );

                // Pop it off the components
                _components[entityIt->type]->pop();

                // Remove the item from the entity
                _entities.removeComponent( id, entityIt );
            }

            SignalList          _addedSignals;
            SignalList          _removedSignals;

            ComponentAssociator _associator;
            Components          _components;
            EntityList          _entities;
        };
    }
}

#endif