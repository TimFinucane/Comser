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
    // TODO: Decide whether we give the functionality for using LocalComponentType instead of ComponentType
  
    /// <summary>
    /// Holds all a group of entities and components.
    /// This functionality is useful for splitting up entities based on when/where they are used.
    /// </summary>
    class Group final : public Scene<GroupSpace::EntityList::EntityId>
    {
    public:
        typedef GroupSpace::EntityList      EntityList;
        typedef GroupSpace::Components      Components;
        typedef GroupSpace::ComponentVector ComponentVector;

        typedef EntityList::EntityId EntityId;
    public:
        Group( const std::initializer_list<ComponentType>& types );
        ~Group();

        /// <summary>
        /// Creates an empty entity, with no components
        /// </summary>
        /// <returns>The handle to that entity</returns>
        EntityId            createEntity()
        {
            return _entities.createEntity();
        }

        /// <summary>
        /// Destroys the entity, plus all it's components
        /// </summary>
        void                destroyEntity( EntityId handle );

        void                clear();

        // <summary>
        // Adds a single component to the given entity
        // </summary>
        // <typeparam name="COMPONENT">Class of item to add</typeparam>
        // <typeparam name="COMARGS">The types of the arguments for the COMPONENT's constructor</typeparam>
        // <param name="id">The entity Id</param>
        // <param name="localType">The local type of the component</param>
        // <param name="args">The args for the components constructor</param>
        template<class COMPONENT, typename... COMARGS>
        void                addComponent( EntityId handle, LocalComponentType localType, COMARGS... args )
        {
            ComponentVector::Index index = _components[localType]->push<COMPONENT, COMARGS...>( std::forward<COMARGS>( args )... );

            _entities.addComponent( handle, localType, index );

            signalAdded( localType, handle, *_components[localType]->get( index ) );
        }

        // <summary>
        // Adds a single component to the given entity. Slightly slower than the version with the LocalComponentType.
        // </summary>
        // <typeparam name="COMPONENT">Class of item to add</typeparam>
        // <typeparam name="COMARGS">The types of the arguments for the COMPONENT's constructor</typeparam>
        // <param name="id">The entity Id</param>
        // <param name="args">The args for the components constructor</param>
        template<class COMPONENT, typename... COMARGS>
        void                addComponent( EntityId handle, COMARGS... args )
        {
            addComponent<COMPONENT>( handle, localType( COMPONENT::id() ), std::forward<COMARGS>( args )... );
        }

        template<class COMPONENT>
        void                removeComponent( EntityId handle )
        {
            EntityList::EntityIterator entityIt = _entities.findComponent( handle, localType( COMPONENT::id() ) );

            _removeComponent<COMPONENT>( handle, entityIt );
        }
        void                removeComponent( EntityId handle, LocalComponentType localType )
        {
            EntityList::EntityIterator entityIt = _entities.findComponent( handle, localType );

            _removeComponent( handle, entityIt );
        }

        template<class COMPONENT>
        COMPONENT*          getComponent( EntityId handle )
        {
            LocalComponentType type = localType( COMPONENT::id() );
            EntityList::EntityIterator it = _entities.findComponent( handle, type );

            if( it == (*_entities[handle]).end() || it->type != type )
                return nullptr;

            return reinterpret_cast<COMPONENT*>( *_components[type]->get( it->index ) );
        }
        Component*          getComponent( EntityId handle, LocalComponentType localType )
        {
            EntityList::EntityIterator it = _entities.findComponent( handle, localType );

            if( it == (*_entities[handle]).end() || it->type != localType )
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

        EntityId                                getId( const EntityList::Iterator it )
        {
            return (unsigned int)std::distance( begin(), it );
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

        SceneType                               type() const
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
            signalRemoved( entityIt->type, id, *_components[entityIt->type]->get( entityIt->index ) );

            // Move this component to last position in its component list
            _swap( entityIt );

            // Pop it off the components list
            _components[entityIt->type]->pop();

            // And remove the item from the entity list
            _entities.removeComponent( id, entityIt );
        }
        void                _removeComponent( EntityList::EntityId id, EntityList::EntityIterator entityIt )
        {
            // Call the signal
            signalRemoved( entityIt->type, id, *_components[entityIt->type]->get( entityIt->index ) );

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

#endif