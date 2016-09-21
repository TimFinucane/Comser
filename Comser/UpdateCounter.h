#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "System.h"
#include "Signal.h"

namespace Comser
{
    class List;

    /*
     * <summary>
     * Will update connections of each UpdateType when
     *  their desired number of 'ticks' have passed.
     * </summary>
     */
    class UpdateCounter
    {
    public:
        typedef unsigned __int32    UpdateType;
        typedef unsigned __int32    TicksPerUpdate;

        typedef Event::Signal::Signal0  Signal;
        typedef Event::Signal::Slot0    Slot;

        /*
         * <summary>
         * A structure that defines the necessary components for using the UpdateCounter
         * </summary>
         */
        struct SignalInfo
        {
            SignalInfo( Signal* sig, unsigned int ticksPerUpdate ) : signal(sig), frequency(ticksPerUpdate), ticks(0) {}
            SignalInfo() : signal(nullptr), frequency(0), ticks(0) {}
            Signal*       signal;

            unsigned __int32    frequency; // Number of ticks before update (1 to 1 by default)
            unsigned __int32    ticks; // Iterator of ticks. When passed == ticks will update
        };
    public:
        /*
         * <summary>
         * Defines each UpdateType, along with how many ticks must pass for it to update
         * </summary>
         * <param name="updates">A vector of each UpdateType with it's corresponding Ticks per Update</param>
         */
        void                        init( std::vector<TicksPerUpdate> updates );
        void                        release();

        void                        update( double ticksPassed );

        sigc::connection            connect( UpdateType type, Slot slot );
    private:
        double                      _delta;

        std::vector<SignalInfo>     _updates;
    };
}

#endif