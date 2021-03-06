#include "SpriteProgram.h"

#include <GraphicsIncludes.h>

#pragma warning( push )
#pragma warning( disable : 4634 )
#pragma warning( disable : 4635 )
#pragma warning( disable : 4201 )
#include <glm/gtc/type_ptr.hpp>
#pragma warning( pop )

using namespace Graphics;

SpriteProgram::SpriteProgram( const FileSystem::File& vertexFile, const FileSystem::File& fragmentFile, glm::mat3 matrix )
    : Program{ Shaders::Shader( GL_VERTEX_SHADER, vertexFile ), Shaders::Shader( GL_FRAGMENT_SHADER, fragmentFile ) },
        _squareInfo( 2, sizeof( Vertex ) ), _spriteInfo( 4, sizeof( Sprite ), 1 )
{
    defineBufferInfo();
    bindVaoState();

    // TODO: Find some better way of storing these
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    // Make the unit square
    std::vector<Vertex> vertices;
    vertices.push_back( { 0.5f, 0.5f, 1.0f, 0.0f } );
    vertices.push_back( { 0.5f,-0.5f, 1.0f, 1.0f } );
    vertices.push_back( {-0.5f, 0.5f, 0.0f, 0.0f } );
    vertices.push_back( {-0.5f,-0.5f, 0.0f, 1.0f } );
    _vertices.update( vertices );

    std::vector<unsigned short> indices{ 0, 2, 1, 1, 2, 3 };
    _indices.update( indices );

    _modelProjPos = uniformLoc( "modelProj" );
    updateMatrix( matrix );
}
SpriteProgram::~SpriteProgram()
{
}

void SpriteProgram::updateMatrix( glm::mat3 matrix )
{
    use();
    glUniformMatrix3fv( 0, 1, false, glm::value_ptr( matrix ) );
    unbind();
}

void SpriteProgram::draw( const Sprites& sprites, const TextureAtlas& atlas )
{
    if( sprites.empty() )
        return;

    use();

    _sprites.update( sprites );
    atlas.bind();

    glDrawElementsInstanced( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, (GLsizei)_sprites.size() );

    unbind();
}

void SpriteProgram::defineBufferInfo()
{
    // Square info is for the vertices of the unit square
    _squareInfo.define( 0, offsetof( Vertex, Vertex::xPos ), GL_FLOAT, 2 ); // Position
    _squareInfo.define( 1, offsetof( Vertex, Vertex::xTex ), GL_FLOAT, 2 ); // Texcoords

    // Sprite info is for the individual sprites being drawn to screen.
    _spriteInfo.define( 0, offsetof( Sprite, Sprite::x ), GL_FLOAT, 2 ); // Position
    _spriteInfo.define( 1, offsetof( Sprite, Sprite::xScale ), GL_FLOAT, 2 ); // Scale
    _spriteInfo.define( 2, offsetof( Sprite, Sprite::angle ), GL_FLOAT, 1 ); // Angle
    _spriteInfo.define( 3, offsetof( Sprite, Sprite::layer ), GL_UNSIGNED_INT, 1 ); // Layer
}
void SpriteProgram::bindVaoState()
{
    bindVao();

    _squareInfo.bind( _indices, _vertices, 0 );
    _spriteInfo.bind( _sprites, 2 );

    // And we're done. Unbind so no one else messes with this accidentally
    unbind();
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}