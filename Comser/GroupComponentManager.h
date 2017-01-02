/*
 * ComponentManager.h
 * Purpose: Holds components used in a Group
 * 
 * Author: Nonanon
 */
#pragma once

#include <vector>

#include "Component.h"
#include "ComponentAssociator.h"

namespace Comser
{
    namespace GroupSpace
    {
        // TODO: More memory efficient form
        /// <summary>
        /// A vector of components of a certain type,
        /// These are NOT sorted, but are usually nearly in
        ///  (chronoligical) order due to the way components are added.
        /// </summary>
        struct ComponentVector
        {
        public:
            typedef unsigned __int32            Index;

            typedef std::vector<Component*>     Vector;
            typedef Vector::iterator            Iterator;
            typedef Vector::const_iterator      ConstIterator;
        public:
            ~ComponentVector();

            // <summary>
            // Add a component onto the stack through new COMPONENT( COMARGS ).
            // </summary>
            // <typeparam name="COMPONENT">The type of component class to make</typeparam>
            // <typeparam name="COMARGS">The paramaters of the component constructor</typeparam>
            // <param name="id">The owning Entity</param>
            // <param name="args">The arguments used to initialize the COMPONENT</param>
            // <returns>The index of the created component</returns>
            template< class COMPONENT, typename... COMARGS >
            Index               push( COMARGS... args )
            {
                // TODO: Better memory
                COMPONENT* comp = new COMPONENT( std::forward<COMARGS>( args )... );
                _vector.push_back( (Component*)comp );

                return (Index)(_vector.size() - 1);
            }

            // Placement construction
            template <typename CONSTRUCTOR>
            Index               push( size_t size, const CONSTRUCTOR& constructor )
            {
                // TODO: Better memory
                void* memory = operator new( size );

                _vector.push_back( constructor( memory ) );

                return (Index)(_vector.size() - 1);
            }

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
                return _vector.begin();
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