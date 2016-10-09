/*
 * ComponentManager.h
 * Purpose: Holds components used in a Group
 * 
 * Author: Nonanon
 */
#pragma once

#include <vector>

#include "Component.h"
#include "Entity.h"
#include "ComponentAssociator.h"

namespace Comser
{
    namespace Group
    {
        // TODO: More memory efficient form
        /// <summary>
        /// A vector of components of a certain type,
        ///  and their respective entity. These are NOT
        ///  sorted, but are usually nearly in (chronoligical) order
        ///  due to the way components are added.
        /// </summary>
        struct ComponentVector
        {
        public:
            typedef unsigned __int32            Index;

            typedef std::vector<Component*>     Vector;
            typedef Vector::iterator            Iterator;
            typedef Vector::const_iterator      ConstIterator;
        public:
            // <summary>
            // Add a component onto the stack through new COMPONENT( COMARGS ).
            // </summary>
            // <typeparam name="COMPONENT">The type of component class to make</typeparam>
            // <typeparam name="COMARGS">The paramaters of the component constructor</typeparam>
            // <param name="id">The owning Entity</param>
            // <param name="args">The arguments used to initialize the COMPONENT</param>
            // <returns>The index of the created component</returns>
            template< class COMPONENT, typename... COMARGS >
            Index               push( COMARGS&... args )
            {
                // TODO: Better memory
                COMPONENT* comp = new COMPONENT( std::forward<COMARGS>( args )... );
                _vector.push_back( (Component*)comp );

                return (Index)(_vector.size() - 1);
            }

            // <summary>
            // Pops item from the back of the vector.
            // Uses COMPONENT to call the component destructor in constant time
            // </summary>
            // <typeparam name="COMPONENT">The class of the component being removed</typeparam>
            template< class COMPONENT >
            void                pop();
            /* TODO doc
             * Pops item from back of vector (Uses virtual destructor,
             *  so not as efficient as templated version).
             */
            void                pop();

            /* TODO doc
             * Swaps the two items.
             */
            void                swap( Index a, Index b );

            /// <returns>Size of the vector of components</returns>
            size_t        size() const
            {
                return _vector.size();
            }

            /* TODO comment
             * Gets the iterator to the item at index
             */
            Iterator            get( Index index )
            {
                return _vector.begin() + index;
            }
            
            /* TODO doc
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
        
        /// <summary>
        /// Holds a vector of ComponentVectors
        /// </summary>
        struct Components
        {
        public:
            Components( size_t count );
            ~Components();

            const ComponentVector*     operator[]( LocalComponentType type ) const
            {
                return &_componentvectors[type.get()];
            }
            ComponentVector*           operator[]( LocalComponentType type )
            {
                return &_componentvectors[type.get()];
            }
        private:
            ComponentVector*           _componentvectors;
        };
    }
}