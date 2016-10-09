#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

namespace Comser
{
    /// An opaque handle, used to access the entity
    ///  of the scene that handed the handle to you.
    /// The underlying type is defined by the scene,
    ///  so don't try handing it to any other scene but
    ///  the one you got it from.
    /// Note that these are meant to be temporary. The entity
    ///  may have been deleted without the entity handle knowing.
    typedef std::shared_ptr<void>   EntityHandle;
}

#endif