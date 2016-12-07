#include "BufferInfo.h"

#include <GraphicsIncludes.h>

using namespace Graphics;
using namespace Graphics::Shaders;

BufferInfo::BufferInfo( unsigned int components, unsigned int step, unsigned int instancing )
    : _components( components ), _step( step ), _instancing( instancing )
{
    _items = new BufferItem[components];
}
BufferInfo::~BufferInfo()
{
    if( _components > 0 )
        delete[] _items;
}

void    BufferInfo::bind( Buffer& array, unsigned int location )
{
    glBindBuffer( GL_ARRAY_BUFFER, array.buffer() );

    for( unsigned int i = 0; i < _components; ++i )
    {
        if( _items[i].type == GL_UNSIGNED_INT || _items[i].type == GL_INT )
            glVertexAttribIPointer( location + i, _items[i].numItems, _items[i].type, _step, (void*)_items[i].offset );
        else
            glVertexAttribPointer( location + i, _items[i].numItems, _items[i].type, GL_FALSE, _step, (void*)_items[i].offset );
        glVertexAttribDivisor( location + i, _instancing ); // Sets up instancing if enabled
    }
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