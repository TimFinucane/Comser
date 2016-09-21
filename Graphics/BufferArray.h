#pragma once

#include <tuple>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/algorithm.hpp>

namespace Graphics
{
    namespace Shaders
    {
        // A buffer array is a group of component arrays that
        //  are stored on the device for use by the Graphics Program.
        class BufferArray
        {
        public:
            BufferArray( unsigned int components, unsigned int step, bool updateable = false, unsigned int instancing = 0 );
            ~BufferArray();

            // Binds the buffer objects to the device context.
            // The [location] defines the starting location to
            //  bind to. Each subsequent buffer binds 1 up.
            void            bind( unsigned int location );

            void            define( unsigned int component, unsigned int offset, unsigned int type, unsigned int numItems )
            {
                _items[component].offset    = offset;
                _items[component].type      = type;
                _items[component].numItems  = numItems;
            }

            // Items must be in the format of a POD of the previously defined components.
            // The size is the number of structures
            void            updateBuffer( void* items, unsigned int count );

            unsigned int    count()
            {
                return _count;
            }
        private:
            struct BufferItem
            {
                unsigned int    offset;
                unsigned int    type;
                unsigned int    numItems;
            };
        private:
            unsigned int    _buffer;

            unsigned int    _components;
            BufferItem*     _items;
            unsigned int    _step;

            unsigned int    _instancing;
            bool            _updateable;

            unsigned int    _count;
        };

        class IndexedBufferArray : public BufferArray
        {
        public:
            IndexedBufferArray( unsigned int components, unsigned int step, bool updateable = false );
            ~IndexedBufferArray();

            void            updateIndices( unsigned short* indices, unsigned int count );

            void            bind( unsigned int location );
            
            unsigned int    count()
            {
                return _indexCount;
            }
        private:
            unsigned int    _indexBuffer;

            unsigned int    _indices;
            unsigned int    _indexCount;
        };
    }
}