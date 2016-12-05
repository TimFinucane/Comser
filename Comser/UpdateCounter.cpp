#define COMSER_EXPORTS
#include "UpdateCounter.h"

#include <vector>

// The maximum time to allow the engine to try to update through (in ticks),
//  If higher than this, the engine will ignore the whole time.
const double MAX_TIME = 20.0f;

using namespace Comser;

UpdateCounter::UpdateCounter( unsigned int size )
    : _updates( size ), _delta( 0.0 )
{
}
UpdateCounter::UpdateCounter( std::initializer_list<unsigned int> orders )
    : _delta( 0.0 ), _updates( orders.begin(), orders.end() ) // Emplace-construct the vector
{    
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
                i->signal.emit();
            }

        }
        --_delta;
    }
}

sigc::connection UpdateCounter::signal( UpdateOrder order, sigc::slot<void> slot )
{
    if( _updates.size() <= order )
        throw std::runtime_error( "Oops, someone tried to access an update counter signal that was outside the initialised range" );

    return _updates[order].signal.connect( slot );
}