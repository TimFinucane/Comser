#pragma once
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <map>

#include "Signal.h"

namespace Comser
{
    /*
     * <summary>
     * A framework that allows you to use event-based design,
     *  based on the use of Signals and Slots.
     * </summary>
     */
    namespace Event
    {
        typedef unsigned __int32                            MessageId;
        typedef std::map<MessageId, Signal::Default>        SignalMap; // TODO: Vector instead?

        class Manager
        {
        public:
            /*
             * <summary>
             * Removes all signals.
             * </summary>
             */
            void                clear()
            {
                for( auto i = _signals.begin(); i != _signals.end(); ++i )
                    i->second.clear();
            
                _signals.clear();
            }

            void                declare( MessageId id )
            {
                _signals[id] = Signal::Default();
            }
            void                remove( MessageId id )
            {
                _signals[id].clear();
                _signals.erase( id );
            }

            /*
             * <summary>
             * Connects a slot to the signal. Assumes that the slot type is correct.
             * </summary>
             * <typeparam name="ARG">The argument type that is passed to the signal when calling it.</typeparam>
             * <param name="message">The message to connect to</param>
             * <param name="slot">The slot being connected</param>
             */
            template <typename ARG>
            sigc::connection    connect( MessageId message, Signal::Slot1<ARG*> slot )
            {
                return _signals[message].connect( slot );
            }
            
            /*
             * <summary>
             * Calls a signal.
             * </summary>
             * <typeparam name="ARG">The argument to give to the signal</typeparam>
             * <param name="message">The message to call</param>
             * <param name="arg">The argument to give to the signal</param>
             */
            template <typename ARG>
            void                operator()( MessageId message, ARG* arg )
            {
                _signals[message]( arg );
            }
        private:
            SignalMap   _signals;
        };
    }
}

#endif