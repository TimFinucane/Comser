#pragma once

#include <string>
#include "UIView.h"

#include <Texture.h>
#include <Font.h>

namespace UI
{
    struct TextModel
    {
    public:
        virtual const std::string&  operator()() = 0;
        virtual void                modify( const std::string& str ) = 0;
    };

    class Text : public View
    {
    public:
        Text( TextModel& text, Graphics::Text::Font& font, unsigned int fontSize = 12 )
            : _text( text ), _font( font ), _fontSize( fontSize )
        {
            refresh();
        }

        void    render( RenderState& renderState, const Transform& transform );

        const  TextModel&   text() const
        {
            return _text;
        }
        void                text( TextModel& model )
        {
            _text = model;
            refresh();
        }
        unsigned int        fontSize() const
        {
            return _fontSize;
        }
        void                fontSize( unsigned int size )
        {
            _fontSize = size;
            refresh();
        }
    private:
        void                refresh();

        Graphics::Texture       _texture;

        Graphics::Text::Font    _font;

        TextModel&              _text;
        unsigned int            _fontSize;
    };
}