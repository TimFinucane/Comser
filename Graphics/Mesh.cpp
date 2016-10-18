#include "Mesh.h"

#include <GraphicsIncludes.h>

using namespace Graphics;

Mesh::Mesh( const void* data, unsigned int size, unsigned int length )
{
    glGenBuffers( 1, &_vertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, _vertexBuffer );

    // TODO: Allow for different usage options
    glBufferData( GL_ARRAY_BUFFER, size * length, data, GL_STATIC_DRAW );
}
Mesh::~Mesh()
{
    glDeleteBuffers( 1, &_vertexBuffer );
}

void IndexedMesh::_createMesh( const unsigned short* indexData, unsigned int length )
{
    glGenBuffers( 1, &_indexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBuffer );
    
    // TODO: Allow for different usage options
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short ) * length, indexData, GL_STATIC_DRAW );
}
IndexedMesh::~IndexedMesh()
{
    glDeleteBuffers( 1, &_indexBuffer );
}