#include "Buffer.h"

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