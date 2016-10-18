#pragma once

#include "Mesh.h"

namespace Graphics
{
    namespace Shaders
    {
        // A buffer array is a group of component arrays that
        //  are stored on the device for use by the Graphics Program.

        /// <summary>
        /// A buffer info class is how you tell the generic graphics base class
        ///  what kind of vertex data is being used, and how to store it.
        /// </summary>
        class BufferInfo
        {
        public:

            /// <param name="components">The number of different buffers being used</param>
            /// <param name="step">The total size of a single vertex</param>
            /// <param name="updateable">Whether or not the vertex buffer will be edited frequently</param>
            /// <param name="instancing">The number of vertices before the next element is loaded into the program. 0 means per vertex.</param>
            BufferInfo( unsigned int components, unsigned int step, unsigned int instancing = 0 );
            ~BufferInfo();

            /// <summary>
            /// Sets up opengl to recieve the buffer described
            ///  by this class
            /// </summary>
            void            bind( Mesh& mesh, unsigned int location );

            /// <summary>
            /// Used to define a specific component of the buffer data
            ///  you plan on passing in.
            /// </summary>
            /// <param name="component">The number this component is into the POD</param>
            /// <param name="offset">The number of bytes offset from the start of the array</param>
            /// <param name="type">The OpenGL type out of which this component is comprised</param>
            /// <param name="numItems">The number of these types present in the component</param>
            void            define( unsigned int component, unsigned int offset, unsigned int type, unsigned int numItems )
            {
                _items[component].offset    = offset;
                _items[component].type      = type;
                _items[component].numItems  = numItems;
            }

        protected:

        private:
            struct BufferItem
            {
                size_t          offset;
                unsigned int    type;
                unsigned int    numItems;
            };
        private:
            unsigned int    _components;
            BufferItem*     _items;
            unsigned int    _step;

            unsigned int    _instancing;
        };

        /// <summary>
        /// An indexed buffer info is a buffer whose elements can be accessed
        ///  by OpenGL by an index, allowing for the same element to be reused.
        /// </summary>
        class IndexedBufferInfo : public BufferInfo
        {
        public:
            // TODO: Instancing with an indexed buffer?
            /// <summary>
            /// Defines the initial info of an IndexedBuffer
            /// </summary>
            IndexedBufferInfo( unsigned int components, unsigned int step );
            ~IndexedBufferInfo();

            void            bind( IndexedMesh& mesh, unsigned int location );
        };
    }
}