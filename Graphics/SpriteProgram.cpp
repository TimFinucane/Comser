#include "SpriteProgram.h"

#include <GraphicsIncludes.h>

using namespace Graphics;

SpriteProgram::SpriteProgram( const FileSystem::File& vertexFile, const FileSystem::File& fragmentFile )
    : Program{ Shaders::Shader( GL_VERTEX_SHADER, vertexFile ), Shaders::Shader( GL_FRAGMENT_SHADER, fragmentFile ) },
        _squareInfo( 2, sizeof( Vertex ) ), _spriteInfo( 3, sizeof( Sprite ), 4 ), 
    _sprites( Buffer::UpdateFrequency::FREQUENTLY ), _vertices( Buffer::UpdateFrequency::ONCE ), _indices( Buffer::UpdateFrequency::ONCE )
{
    _squareInfo.define( 0, offsetof( Vertex, Vertex::xPos ), GL_FLOAT, 2 );
    _squareInfo.define( 1, offsetof( Vertex, Vertex::xTex ), GL_FLOAT, 2 );

    _spriteInfo.define( 0, offsetof( Sprite, Sprite::x ), GL_FLOAT, 2 );
    _spriteInfo.define( 1, offsetof( Sprite, Sprite::xScale ), GL_FLOAT, 2 );
    _spriteInfo.define( 2, offsetof( Sprite, Sprite::layer ), GL_UNSIGNED_INT, 1 );

    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glEnableVertexAttribArray( 2 );
    glEnableVertexAttribArray( 3 );
    glEnableVertexAttribArray( 4 );

    // TODO: Find some better way of storing these
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    std::vector<Vertex> vertices;
    vertices.push_back( { 0.5f, 0.5f, 0.0f, 0.0f } );
    vertices.push_back( { 0.5f,-0.5f, 0.0f, 1.0f } );
    vertices.push_back( {-0.5f, 0.5f, 1.0f, 0.0f } );
    vertices.push_back( {-0.5f,-0.5f, 1.0f, 1.0f } );
    _vertices.update( vertices );

    std::vector<unsigned short> indices{ 0, 1, 2, 1, 2, 3 };
    _indices.update( indices );
}

void SpriteProgram::draw( const Sprites& sprites, const TextureAtlas& atlas )
{
    _squareInfo.bind( _indices, _vertices, 0 );

    _sprites.update( sprites );
    _spriteInfo.bind( _sprites, 2 );

    atlas.bind();

    glDrawElementsInstanced( GL_TRIANGLES, 2, GL_UNSIGNED_SHORT, nullptr, (GLsizei)_sprites.size() );
}