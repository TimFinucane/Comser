#define COMSER_EXPORTS
#include "UpdateCounter.h"

#include <vector>

#include "System.h"

// The maximum time to allow the engine to try to update through (in ticks),
//  If higher than this, the engine will ignore the whole time.
#define MAX_TIME 20.0f

using namespace Comser;

void UpdateCounter::init( std::vector<TicksPerUpdate> updates )
{
    _delta = 0;

    // Allocate signals TODO: Memory
    // Create this many different updates
    _updates.resize( updates.size() );

    // Fill them with the ticks per update, ticks that have passed since last update, and a signal.
    for( unsigned __int32 i = 0; i < updates.size(); ++i )
    {
        _updates[i].frequency = updates[i];
        _updates[i].ticks = 0;
        // Adding signals here to reduce runtime memory calls
        // TODO: Allocate
        _updates[i].signal = new Signal();
    }
}
void UpdateCounter::release()
{
    for( auto i = _updates.begin(); i != _updates.end(); ++i )
    {
        delete i->signal;
    }
}

void UpdateCounter::update( double ticks )
{
    // Find out how much time has passed
    _delta += ticks;

    // If the time is too large to process, then dont
    if( _delta > MAX_TIME )
    {
        _delta = 0.0f;
        return;
    }
    // Otherwise, go through each tick until delta is gone
    // TODO: Think of better way?
    while( _delta > 1 )
    {
        // Update each individual signal
        for( auto i = _updates.begin(); i != _updates.end(); ++i )
        {
            // Increment the counter of ticks since
            //  last update
            i->ticks++;
            // If enough ticks has passed, the system will update
            if( i->ticks == i->frequency )
            {
                i->ticks = 0;
                i->signal->emit();
            }

        }
        --_delta;
    }
}

sigc::connection    UpdateCounter::connect( UpdateType type, Slot slot )
{
    _ASSERT( _updates.size() > type );

    return _updates[type].signal->connect( slot );
}