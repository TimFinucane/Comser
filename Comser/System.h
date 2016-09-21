#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include <sigc++/sigc++.h>

namespace Comser
{
    // Purpose: A system is a process or series of processes that can hook to the engine for updating.

    // NB: To update, you simply hook an update function to the engine with the desired priority
    // Also can hook to an entity with desired node being enabled or disabled
    class System abstract : public sigc::trackable
    {

    };
}

#endif