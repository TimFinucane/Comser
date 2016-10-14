#pragma once

#include <initializer_list>

namespace Graphics
{
    template <typename VERTEX>
    struct Mesh
    {
    public:
        Mesh( std::initializer_list<VERTEX> vertices, std::initializer_list<unsigned short> indices )
            : mesh( vertices.size(), indices.size() )
        {
            unsigned int i = 0;
            for( auto it = vertices.begin(); it != vertices.end(); ++i, ++it )
                _vertices[i] = *it;
            
            i = 0;
            for( auto it = indices.begin(); it != indices.end(); ++i, ++it )
                _indices[i] = *it;
        }
        Mesh( unsigned int vertexCount, unsigned int indexCount )
            : _vertexCount( vertexCount ), _indexCount( indexCount )
        {
            _vertices = new VERTEX[vertexCount];
            _indices = new unsigned short[indexCount];
        }
        ~Mesh()
        {
            delete[] _vertices;
            delete[] _indices;
        }

        VERTEX*         vertices()
        {
            return _vertices;
        }
        unsigned short* indices()
        {
            return _indices;
        }
    private:
        unsigned int        _vertexCount;
        unsigned int        _indexCount;

        VERTEX*             _vertices;
        unsigned short*     _indices;
    };
}