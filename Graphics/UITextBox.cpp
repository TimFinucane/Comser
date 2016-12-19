/*#include "UITextBox.h"

#include "UIRenderInfo.h"

using namespace UI;

void Text::render( RenderState& renderState, Transform& transform )
{
    renderState.program.draw( transform.matrix, _texture );
}

void Text::refresh()
{
    _font.useSize( _fontSize );

    // TODO: Implement proper conversion from utf8 to UCS4
    std::vector<unsigned int> chars( _text().begin(), _text().end() );
    auto line = _font.genLine( &chars[0], chars.size(), true );

    _texture = Graphics::Texture( line.file );
}*/