/*
 * EntityList.h
 * Purpose: Handles the Entities held in a Group
 * 
 * author: nonanon
 */
#pragma once
#include <vector>

#include "GroupComponentManager.h"

namespace Comser
{
    namespace GroupSpace
    {
        /// <summary>
        /// Stores Entities as a vector of <c>ComponentLocalType</c> and
        ///  indices into their respective vectors.
        /// </summary> 
        class EntityList
        {
        public:
            /// <summary>
            /// Holds info about a single component in an entity.
            /// </summary>
            struct ComponentDef
            {
                LocalComponentType      type;
                ComponentVector::Index  index;

                bool operator ==( const ComponentDef& def )
                {
                    return type == def.type && index == def.index;
                }
            };

            typedef unsigned int                EntityId;

            // TODO: Use Map instead? Or something easier to insert and erase from?
            // In the group, an entity holds info about the location of all of it's components
            typedef std::vector<ComponentDef>   Entity;

            // Iterator for a component in a single entity
            typedef Entity::iterator            EntityIterator;
            typedef Entity::const_iterator      EntityConstIterator;

            // Iterators and stuff for whole vector
            typedef std::vector<Entity>         Vector;
            typedef Vector::iterator            Iterator;
            typedef Vector::const_iterator      ConstIterator;
        public:
            /// <summary>
            /// Creates an entity with no component references in it.
            /// </summary>
            /// <returns>The entities' id</returns>
            EntityId                    createEntity();

            /// <summary>
            /// Destroys the entity, but not it's components.
            /// </summary>
            void                        destroyEntity( EntityId id );

            /// <summary>
            /// Adds a component reference to the entity
            /// </summary>
            /// <param name="id">The entity</param>
            /// <param name="type">The components' type</param>
            /// <param name="index">The index to the component given by the <c>ComponentVector</c></param>
            void                        addComponent( EntityId id, LocalComponentType type, ComponentVector::Index index );

            /// <summary>
            /// Removes a component reference from the entity, but doesn't
            ///  actually destroy that component.
            /// </summary>
            /// <param name="id">The entity</param>
            /// <param name="type">The components' type</param>
            void                        removeComponent( EntityId id, LocalComponentType type );

            /// <summary>
            /// Removes a component reference from the entity, but
            ///  doesn't actually destroy the component.
            /// </summary>
            /// <param name="id">The entity</param>
            /// <param name="iterator">The iterator to that specific component reference</param>
            void                        removeComponent( EntityId id, EntityIterator iterator );

            /// <summary>
            /// Finds the component info (effectively the location) using
            ///  the type and entity id.
            /// </summary>
            /// <returns>The iterator to that component in the entity</returns>
            EntityIterator              findComponent( EntityId id, LocalComponentType type );

            size_t                size() const
            {
                return _entities.size();
            }

            Iterator                    operator[]( unsigned int i )
            {
                return (_entities.begin() + i );
            }

            const ConstIterator         begin() const
            {
                return _entities.begin();
            }
            const Iterator              begin()
            {
                return _entities.begin();
            }
            const ConstIterator         end() const
            {
                return _entities.end();
            }
            const Iterator              end()
            {
                return _entities.end();
            }

            const EntityConstIterator   begin( EntityId id ) const
            {
                return _entities[id].begin();
            }
            const EntityIterator        begin( EntityId id )
            {
                return _entities[id].begin();
            }
            const EntityConstIterator   end( EntityId id ) const
            {
                return _entities[id].end();
            }
            const EntityIterator        end( EntityId id )
            {
                return _entities[id].end();
            }
        private:
            std::vector<Entity>         _entities;
            std::vector<EntityId>       _emptySlots;
        };
    }
}