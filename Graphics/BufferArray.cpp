#include "BufferArray.h"

#include <GraphicsIncludes.h>

using namespace Graphics::Shaders;

BufferArray::BufferArray( unsigned int components, unsigned int step, bool updateable, unsigned int instancing )
    : _components( components ), _step( step ), _updateable( updateable ), _instancing( instancing )
{
    //glCreateBuffers( 1, &_buffer ); 4.5 only :(
    glGenBuffers( 1, &_buffer );
}
BufferArray::~BufferArray()
{
    glDeleteBuffers( 1, &_buffer );
}

void    BufferArray::bind( unsigned int location )
{
    glBindBuffer( GL_ARRAY_BUFFER, _buffer );

    for( unsigned int i = 0; i < _components; ++i )
    {
        glVertexAttribPointer( location + i, _items[i].numItems, _items[i].type, GL_FALSE, _step, (void*)_items[i].offset );
        glVertexAttribDivisor( location + i, _instancing );
    }
}

void    BufferArray::updateBuffer( void* items, unsigned int count )
{
    glBindBuffer( GL_ARRAY_BUFFER, _buffer );

    glBufferData( GL_ARRAY_BUFFER, count * _step, items, _updateable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );

    _count = count;
}

IndexedBufferArray::IndexedBufferArray( unsigned int components, unsigned int step, bool updateable )
    : BufferArray( components, step, updateable )
{
    glGenBuffers( 1, &_indexBuffer );
}
IndexedBufferArray::~IndexedBufferArray()
{
    glDeleteBuffers( 1, &_indexBuffer );
}

void    IndexedBufferArray::updateIndices( unsigned short* indices, unsigned int count )
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBuffer );

    glBufferData( GL_ARRAY_BUFFER, count * sizeof( unsigned short ), indices, GL_STATIC_DRAW );

    _indexCount = count;
}

void    IndexedBufferArray::bind( unsigned int location )
{
    BufferArray::bind( location );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBuffer );
}