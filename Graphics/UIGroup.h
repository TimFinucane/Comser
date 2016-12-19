#pragma once
#include <vector>
#include <memory>

#include "UIView.h"
#include "UITransform.h"

namespace UI
{
    // A group holds a bunch of views along with their positions in the group space.
    // It can be derived from (in protected manner) for specific functionality,
    //  or instantiated for normal function
    class Group : public View
    {
    public:
        struct ViewItem
        {
            Transform   transform;
            View*       view;
        };
        using Vector = std::vector<ViewItem>;

    public:
        // TODO: Resizing notifications

        void        render( RenderState& renderState, const Transform& transform )
        {
            renderItems( renderState, transform );
        }
        Vector&     items()
        {
            return views;
        }

    protected:
        void    renderItems( RenderState& renderState, const Transform& transform )
        {
            for( auto i = views.begin(); i != views.end(); ++i )
                i->view->render( renderState, Transform{ transform() * i->transform() } );
        }

        Vector  views;
    };
};