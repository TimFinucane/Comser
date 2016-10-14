#include "BufferArray.h"

#include <GraphicsIncludes.h>

using namespace Graphics::Shaders;

BufferInfo::BufferInfo( unsigned int components, unsigned int step, bool updateable, unsigned int instancing )
    : _components( components ), _step( step ), _updateable( updateable ), _instancing( instancing )
{
}
BufferInfo::~BufferInfo()
{
}

void    BufferInfo::prepareBind( unsigned int location )
{
    for( unsigned int i = 0; i < _components; ++i )
    {
        glVertexAttribPointer( location + i, _items[i].numItems, _items[i].type, GL_FALSE, _step, (void*)_items[i].offset );
        glVertexAttribDivisor( location + i, _instancing );
    }

    if( _instancing )
        glInstance // TODO:
}

IndexedBufferInfo::IndexedBufferInfo( unsigned int components, unsigned int step, bool updateable )
    : BufferInfo( components, step, updateable )
{
}
IndexedBufferInfo::~IndexedBufferInfo()
{
}

void    IndexedBufferArray::bind( unsigned int location )
{
    BufferInfo::bind( location );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBuffer );
}