#include "ObjectArray.h"

#include <GraphicsIncludes.h>

using namespace Graphics;

Buffer::Buffer( UpdateFrequency frequency )
    : _frequency( frequency )
{
    glGenBuffers( 1, &_buffer );
}
Buffer::Buffer( UpdateFrequency frequency, size_t length, void* data )
    : Buffer( frequency )
{
    recreate( length, data );
}
Buffer::~Buffer()
{
    glDeleteBuffers( 1, &_buffer );
}

void Buffer::recreate( size_t length, void* data )
{
    glBindBuffer( GL_ARRAY_BUFFER, _buffer );
    glBufferData( GL_ARRAY_BUFFER, length, data,
                  _frequency == UpdateFrequency::ONCE ? GL_STATIC_DRAW :
                  _frequency == UpdateFrequency::OCCASIONALLY ? GL_DYNAMIC_DRAW : GL_STREAM_DRAW );
}
void Buffer::replace( size_t start, size_t length, void* data )
{
    glBindBuffer( GL_ARRAY_BUFFER, _buffer );
    glBufferSubData( GL_ARRAY_BUFFER, start, length, data );
}