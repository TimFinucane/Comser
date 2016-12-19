#include "UIImage.h"

#include "UIRenderInfo.h"
#include "UITransform.h"

using namespace UI;

void Image::render( RenderState& renderState, const Transform& transform )
{
    renderState.program.draw( transform.matrix, texture );
}