/**
  * ComponentManager.h
  * Purpose: Holds components used in a Group
  * 
  * @author nonanon
  */
#pragma once
#ifndef COMPONENT_MAP_H
#define COMPONENT_MAP_H

#include <vector>

#include "Component.h"
#include "Entity.h"

namespace Comser
{
    namespace Scene
    {
        /*
         * <summary>
         * An explicitly typed integer used to reference
         *  a certain component type for a single group.
         * </summary>
         */
        struct    LocalComponentType
        {
            friend struct ComponentAssociator;
        public:
            LocalComponentType()
                : _type( 0 )
            {}

            unsigned __int16 get() const
            {
                return _type;
            }

            bool    operator > ( const LocalComponentType b ) const
            {
                return _type > b._type;
            }
            bool    operator < ( const LocalComponentType b ) const
            {
                return _type < b._type;
            }
            bool    operator ==( const LocalComponentType b ) const
            {
                return _type == b._type;
            }
            bool    operator !=( const LocalComponentType b ) const
            {
                return _type != b._type;
            }
        private:
            LocalComponentType( unsigned short type )
                : _type( type )
            {}
            /*
             * <summary>
             * Sets the LocalComponentType to the type.
             * Is private so only the Associator can do this
             * </summary>
             */
            void operator =( unsigned short type )
            {
                _type = type;
            }

            unsigned __int16 _type;
        };

        /*
         * <summary>
         * Class that creates the association with a component's global ComponentType,
         *  and the LocalComponentType for that component in one Group.
         * </summary>
         */
        struct ComponentAssociator
        {
        public:
            /*
             * Give it the ComponentTypes, it forms the LocalComponentTypes
             */
            ComponentAssociator( unsigned int count, ComponentType* types );

            /*
             * Put in the global ComponentType, it gets the LocalComponentType
             */
            LocalComponentType operator[]( ComponentType type ) const;
        private:
            unsigned int    _count;
            ComponentType*  _types;
        };

        /*
         * <summary>
         * A vector of components of a certain type,
         *  and their respective entity. These are NOT
         *  sorted, but are usually nearly in (chronoligical) order
         *  due to the way components are added.
         * </summary>
         * TODO: More memory efficient form
         */
        struct ComponentVector
        {
        public:
            typedef unsigned __int16            Index;
            struct ComponentInfo
            {
                ComponentInfo()
                {}
                ComponentInfo( EntityId i, Component* c )
                    : entity( i ), component( c )
                {}

                EntityId    entity;
                Component*  component;
            };

            typedef std::vector<ComponentInfo>  Vector;
            typedef Vector::iterator            Iterator;
            typedef Vector::const_iterator      ConstIterator;
        public:
            /*
             * <summary>
             * Add a component onto the stack through new COMPONENT( COMARGS ).
             * </summary>
             * <typeparam name="COMPONENT">The type of component class to make</typeparam>
             * <typeparam name="COMARGS">The paramaters of the component constructor</typeparam>
             * <param name="id">The owning Entity</param>
             * <param name="args">The arguments used to initialize the COMPONENT</param>
             * <returns>The index of the created component</returns>
             */
            template< class COMPONENT, typename... COMARGS >
            Index               push( EntityId id, COMARGS&... args )
            {
                // TODO: Better memory
                COMPONENT* comp = new COMPONENT( std::forward<COMARGS>( args )... );
                ComponentInfo info = ComponentInfo( id, (Component*)comp );
                _vector.push_back( info );

                return (Index)(_vector.size() - 1);
            }

            /*
             * <summary>
             * Pops item from the back of the vector.
             * Uses COMPONENT to call the component destructor in constant time
             * </summary>
             * <typeparam name="COMPONENT">The class of the component being removed</typeparam>
             */
            template< class COMPONENT >
            void                pop();
            /*
             * Pops item from back of vector (Uses virtual destructor,
             *  so not as efficient as templated version).
             */
            void                pop();

            /*
             * Swaps the two items.
             */
            void                swap( Index a, Index b );

            /*
             * <returns>Size of the vector of components</returns>
             */
            unsigned int        size() const
            {
                return _vector.size();
            }

            /*
             * Gets the iterator to the item at index
             */
            Iterator            get( Index index )
            {
                return _vector.begin() + index;
            }
            
            /*
             * Begin and End iterators for For loops.
             */
            const ConstIterator begin() const
            {
                return _vector.begin();
            }
            const Iterator      begin()
            {
                _vector.begin();
            }
            const ConstIterator end() const
            {
                return _vector.end();
            }
            const Iterator      end()
            {
                _vector.end();
            }

        private:
            Vector          _vector;
        };
        
        /*
         * <summary>
         * Holds a vector of ComponentVectors
         * </summary>
         */
        struct Components
        {
        public:
            Components( unsigned int count );

            const ComponentVector*  operator[]( LocalComponentType type ) const
            {
                return &_componentvectors[type.get()];
            }
            ComponentVector*        operator[]( LocalComponentType type )
            {
                return &_componentvectors[type.get()];
            }
        private:
            ComponentVector*    _componentvectors;
        };
    }
}

#endif