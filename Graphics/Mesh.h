#pragma once

#include <initializer_list>

namespace Graphics
{
    template <typename VERTEX>
    struct MeshData
    {
    public:
        MeshData( std::initializer_list<VERTEX> vertices )
            : MeshData( vertices.size() )
        {
            unsigned int i = 0;
            for( auto it = vertices.begin(); it != vertices.end(); ++i, ++it )
                _vertices[i] = *it;
        }
        MeshData( unsigned int vertexCount, unsigned int indexCount )
            : _vertexCount( vertexCount )
        {
            _vertices = new VERTEX[vertexCount];
        }
        ~MeshData()
        {
            delete[] _vertices;
        }

        void            clear()
        {
            if( _vertices == nullptr )
            {
                delete[] _vertices;

                _vertexCount = 0;

                _vertices = nullptr;
            }
        }
        void            resize( unsigned int vertexCount )
        {
            clear();

            _vertexCount = vertexCount;

            _vertices = new VERTEX[vertexCount];
        }

        const VERTEX*           vertices() const
        {
            return _vertices;
        }
        VERTEX*                 vertices()
        {
            return _vertices;
        }

        unsigned int            vertexCount() const
        {
            return _vertexCount;
        }
    private:
        unsigned int        _vertexCount;

        VERTEX*             _vertices;
    };

    template <typename VERTEX>
    struct IndexedMeshData
    {
    public:
        IndexedMeshData( std::initializer_list<VERTEX> vertices, std::initializer_list<unsigned short> indices )
            : _vertices( vertices ), _indexCount( indices.size() )
        {
            _indices = new unsigned short[_indexCount];

            unsigned int i = 0;
            for( auto it = indices.begin(); it != indices.end(); ++i, ++it )
                _indices[i] = *it;
        }
        IndexedMeshData( unsigned int vertexCount, unsigned int indexCount )
            : _vertices( vertexCount ), _indexCount( indices.size() )
        {
            _indices = new unsigned short[indexCount];
        }
        ~IndexedMeshData()
        {
            delete[] _indices;
        }

        void            clear()
        {
            _vertices.clear();

            if( _indices == nullptr )
            {
                delete[] _indices;

                _indexCount = 0;

                _indices = nullptr;
            }
        }
        void            resize( unsigned int vertexCount, unsigned int indexCount )
        {
            clear();

            _vertices.resize( vertexCount );

            _indexCount = indexCount;
            _indices = new unsigned short[indexCount];
        }

        const MeshData<VERTEX>& vertexData() const
        {
            return _vertices;
        }

        const unsigned short*   indices() const
        {
            return _indices;
        }
        unsigned short*         indices()
        {
            return _indices;
        }

        unsigned int            indexCount() const
        {
            return _indexCount;
        }
    private:
        MeshData<VERTEX>    _vertices;

        unsigned int        _indexCount;
        unsigned short*     _indices;
    };

    struct Mesh
    {
        template <typename VERTEX>
        Mesh( const MeshData<VERTEX>& meshData )
            : Mesh( meshData.vertices(), sizeof( VERTEX ), meshData.vertexCount )
        {
        }
        ~Mesh();

        unsigned int    vertexBuffer()
        {
            return _vertexBuffer;
        }

    private:
        Mesh( const void* data, unsigned int size, unsigned int length );

        unsigned int    _vertexBuffer;
    };

    struct IndexedMesh
    {
        template <typename VERTEX>
        IndexedMesh( const IndexedMeshData<VERTEX>& meshData )
            : _vertexBuffer( meshData.vertexData() )
        {
            _createMesh( meshData.indices(), meshData.indexCount() ),
        }
        ~IndexedMesh();

        unsigned int    indexBuffer()
        {
            return _indexBuffer;
        }
        Mesh&           vertexBuffer()
        {
            return _vertexBuffer;
        }
    private:
        void            _createMesh( const unsigned short* indexData, unsigned int length );

        Mesh            _vertexBuffer;
        unsigned int    _indexBuffer;
    };
}