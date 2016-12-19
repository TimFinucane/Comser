#pragma once

#include "UIView.h"

#include "Texture.h"

namespace UI
{
    // Declaration so we can provide our explicit template specialisation
    template <class Item>
    class Border;

    // Define a border with no item
    template <>
    class Border<void> : public View
    {
    public:
        enum class Style
        {
            FULL_LINE
        };

    public:
        Border( float thickness = 0.1f, Style style = Style::FULL_LINE );

        void    render( RenderState& renderState, const Transform& transform );

        Style   style;
        float   thickness;

    protected:
        // Reduces the transform area by the thickness of the border
        Transform   reduce( const Transform& transform );

        Graphics::Texture _black;
    };

    // Define a generalised border with item, that relies on our itemless border
    template <class ITEM>
    class Border : public Border<void>
    {
    public:
        using Item = ITEM;
    public:
        Border( float thickness = 0.1f, Style style = Style::FULL_LINE )
            : item(), Border<void>( thickness, style )
        {
        }
        Border( Item&& item, float thickness, Style style = Style::FULL_LINE )
            : item( std::move( item ) ), Border<void>( thickness, style )
        {
        }
        Border( const Item& item, float thickness, Style style = Style::FULL_LINE )
            : item( item ), Border<void>( thickness, style )
        {
        }

        void    render( RenderState& renderState, const Transform& transform )
        {
            Border<void>::render( renderState, transform );
            item.render( renderState, reduce( transform ) );
        }

        Item    item;
    };
}