#include "UIBorder.h"

#include "UITransform.h"
#include "UIRenderInfo.h"

using namespace UI;

Border<void>::Border( float thickness, Style style )
    : thickness( thickness ), style( style )
{
    unsigned int pixel = 0xFF000000;
    _black = Graphics::Texture( Graphics::Image( 1, 1, 8, 4, reinterpret_cast<unsigned char*>(&pixel) ) );
}

void Border<void>::render( RenderState& renderState, const Transform& transform )
{
    float edge = 0.5f - thickness / 2.0f;

    // Render the four corners
    glm::mat4 left   = glm::scale( glm::translate( transform(), glm::vec3( -edge, +0.0f, 0.0f ) ), glm::vec3( thickness, 1.0f, 1.0f ) );
    glm::mat4 top    = glm::scale( glm::translate( transform(), glm::vec3( +0.0f, +edge, 0.0f ) ), glm::vec3( 1.0f, thickness, 1.0f ) );

    glm::mat4 right  = glm::scale( glm::translate( transform(), glm::vec3( +edge, +0.0f, 0.0f ) ), glm::vec3( thickness, 1.0f, 1.0f ) );
    glm::mat4 bottom = glm::scale( glm::translate( transform(), glm::vec3( +0.0f, -edge, 0.0f ) ), glm::vec3( 1.0f, thickness, 1.0f ) );

    renderState.program.draw( left,   _black );
    renderState.program.draw( top,    _black );
    renderState.program.draw( right,  _black );
    renderState.program.draw( bottom, _black );
}

Transform Border<void>::reduce( const Transform& transform )
{
    float expand = 1.0f - thickness * 2.0f;

    return Transform{ glm::scale( transform(), glm::vec3( expand, expand, 1.0f ) ) };
}