#pragma once

#include <vector>
#include <sigc++/sigc++.h>

namespace Comser
{
    class List;

    // The update count
    typedef unsigned int    UpdateOrder;
    typedef unsigned int    TicksPerUpdate;

    /// <summary>
    /// Will update connections of each UpdateType when
    ///  their desired number of 'ticks' have passed.
    /// </summary>
    class UpdateCounter
    {
    public:
        typedef sigc::signal<void>      Signal;

        /// <summary>
        /// A structure that defines the necessary components for using the UpdateCounter
        /// </summary>
        struct SignalInfo
        {
            SignalInfo( unsigned int ticksPerUpdate ) : signal(), frequency(ticksPerUpdate), ticks(0) {}
            SignalInfo() : signal(), frequency(1), ticks(0) {}

            SignalInfo( SignalInfo&& sig ) noexcept
                : signal( std::move( sig.signal ) ), frequency( sig.frequency ), ticks( sig.ticks )
            {
                sig.frequency = 0;
                sig.ticks = 0;
            }
            SignalInfo& operator =( SignalInfo&& sig ) noexcept
            {
                signal = std::move( sig.signal );
                frequency = std::move( sig.frequency );
                ticks = std::move( sig.ticks );
            }
            SignalInfo( const SignalInfo& sig ) = delete;
            SignalInfo& operator =( const SignalInfo& sig ) = delete;

            Signal              signal;

            unsigned int        frequency; // Number of ticks before update (1 to 1 by default)
            unsigned int        ticks; // Iterator of ticks. When passed == ticks will update
        };
    public:
        /// <summary>
        /// Sets up the number of update categories that the counter will have
        /// </summary>
        UpdateCounter( unsigned int orders );
        /// <summary>
        /// Sets up the number of update categories that the counter will have, along with the frequencies of each
        /// </summary>
        UpdateCounter( std::initializer_list<unsigned int> orders );

        ~UpdateCounter() {}

        /// <summary>
        /// Sets the number of ticks that the given UpdateOrder will go through before updating
        /// </summary>
        void                        ticksPerUpdate( UpdateOrder order, unsigned int ticks )
        {
            SignalInfo& update = _updates[order];

            if( ticks == 0 )
                _zeroUpdates.push_back( &update );
            else if( update.frequency == 0 )
                _zeroUpdates.erase( std::find( _zeroUpdates.begin(), _zeroUpdates.end(), &update ) );
            update.frequency = ticks;
        }

        void                        update( double ticksPassed );

        sigc::connection            signal( UpdateOrder order, sigc::slot<void> slot );
    private:
        double                      _delta;

        std::vector<SignalInfo*>    _zeroUpdates;
        std::vector<SignalInfo>     _updates;
    };
}