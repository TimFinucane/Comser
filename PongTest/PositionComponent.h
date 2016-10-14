#pragma once
#include <Component.h>

namespace Components
{
    struct Position : public Comser::Component
    {
        double x;
        double y;

        COMSER_COMPONENT_DECLARE
    };
}