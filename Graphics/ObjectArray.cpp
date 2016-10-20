#include "ObjectArray.h"

#include <GraphicsIncludes.h>

using namespace Graphics;

_ObjectArrayBase::_ObjectArrayBase( UpdateFrequency frequency )
    : _frequency( frequency )
{
    glGenBuffers( 1, &_buffer );
}
_ObjectArrayBase::_ObjectArrayBase( UpdateFrequency frequency, size_t length, void* data )
    : _ObjectArrayBase( frequency )
{
    recreate( length, data );
}
_ObjectArrayBase::~_ObjectArrayBase()
{
    glDeleteBuffers( 1, &_buffer );
}

void _ObjectArrayBase::recreate( size_t length, void* data )
{
    glBindBuffer( GL_ARRAY_BUFFER, _buffer );
    glBufferData( GL_ARRAY_BUFFER, length, data,
                  _frequency == UpdateFrequency::ONCE ? GL_STATIC_DRAW :
                  _frequency == UpdateFrequency::OCCASIONALLY ? GL_DYNAMIC_DRAW : GL_STREAM_DRAW );
}
void _ObjectArrayBase::replace( size_t start, size_t length, void* data )
{
    glBindBuffer( GL_ARRAY_BUFFER, _buffer );
    glBufferSubData( GL_ARRAY_BUFFER, start, length, data );
}