#include "BufferInfo.h"

#include <GraphicsIncludes.h>

using namespace Graphics;
using namespace Graphics::Shaders;

BufferInfo::BufferInfo( unsigned int components, unsigned int step, unsigned int instancing )
    : _components( components ), _step( step ), _instancing( instancing )
{
}
BufferInfo::~BufferInfo()
{
}

void    BufferInfo::bind( Buffer& array, unsigned int location )
{
    for( unsigned int i = 0; i < _components; ++i )
    {
        glVertexAttribPointer( location + i, _items[i].numItems, _items[i].type, GL_FALSE, _step, (void*)_items[i].offset );
        glVertexAttribDivisor( location + i, _instancing ); // Sets up instancing if enabled
    }

    glBindBuffer( GL_ARRAY_BUFFER, array.buffer() );
}

IndexedBufferInfo::IndexedBufferInfo( unsigned int components, unsigned int step )
    : BufferInfo( components, step )
{
}
IndexedBufferInfo::~IndexedBufferInfo()
{
}

void    IndexedBufferInfo::bind( Buffer& indices, Buffer& vertices, unsigned int location )
{
    BufferInfo::bind( vertices, location );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indices.buffer() );
}