#include "BasicProgram.h"

#include <GraphicsIncludes.h>

#pragma warning( push )
#pragma warning( disable : 4634 )
#pragma warning( disable : 4635 )
#pragma warning( disable : 4201 )
#include <glm/gtc/type_ptr.hpp>
#pragma warning( pop )

#include "Shader.h"

using namespace Graphics;

BasicProgram::BasicProgram( const FileSystem::File& vertex, const FileSystem::File& fragment )
    : Program{ Shaders::Shader( GL_VERTEX_SHADER, vertex ), Shaders::Shader( GL_FRAGMENT_SHADER, fragment ) },
    _squareInfo( 2, sizeof( Vertex ) )
{
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    // Define our VAO info
    _squareInfo.define( 0, offsetof( Vertex, Vertex::xPos ), GL_FLOAT, 2 );
    _squareInfo.define( 1, offsetof( Vertex, Vertex::xTex ), GL_FLOAT, 2 );

    // Bind this info to our VAO
    bindVao();

    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );

    _squareInfo.bind( _indices, _vertices, 0 );

    // And we're done. Unbind so no one else messes with this accidentally
    unbind();
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    // Make the unit square around centre 0.0f, 0.0f
    std::vector<Vertex> vertices;
    vertices.push_back( { 0.5f, 0.5f, 1.0f, 0.0f } );
    vertices.push_back( { 0.5f,-0.5f, 1.0f, 1.0f } );
    vertices.push_back( { -0.5f, 0.5f, 0.0f, 0.0f } );
    vertices.push_back( { -0.5f,-0.5f, 0.0f, 1.0f } );
    _vertices.update( vertices );

    // And the indices for the square
    std::vector<unsigned short> indices{ 0, 2, 1, 1, 2, 3 };
    _indices.update( indices );

    _modelProjPos = uniformLoc( "modelProj" );
}

void BasicProgram::draw( const glm::mat4& matrix, const Texture& texture )
{
    use();

    texture.bind();
    glUniformMatrix4fv( _modelProjPos, 1, false, glm::value_ptr( matrix ) );

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr );

    unbind();
}