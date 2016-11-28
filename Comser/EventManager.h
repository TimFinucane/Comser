#pragma once
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <map>
#include <sigc++/sigc++.h>

#include "Message.h"

namespace Comser
{
    /// <summary>
    /// A framework that allows you to use event-based design,
    ///  based on the use of Signals and Slots.
    /// </summary>
    namespace Event
    {
        class Manager
        {
        private:
            typedef sigc::signal<void, const void*>     Signal;
            typedef std::map<MessageId, Signal>         SignalMap;
        public:
            /// <summary>
            /// Removes all signals.
            /// </summary>
            void                clear()
            {
                _signals.clear();
            }

            template <struct Message>
            void                clear()
            {
                clear( Message::id() );
            }
            void                clear( MessageId id )
            {
                _signals.erase( id );
            }

            // <summary>
            // Connects a slot to the signal. Assumes that the slot type is correct.
            // </summary>
            template <struct Message>
            sigc::connection    connect( Message::Slot slot )
            {
                return _signals[Message::id()].connect( slot );
            }
            
            /*
             * <summary>
             * Calls a signal.
             * </summary>
             * <typeparam name="ARG">The argument to give to the signal</typeparam>
             * <param name="message">The message to call</param>
             * <param name="arg">The argument to give to the signal</param>
             */
            template <MessageId ID, typename ARG>
            void                operator()( const Message<ID, ARG>& message )
            {
                _signals[ID]( message.arg );
            }
            template <MessageId ID>
            void                operator()( const Message<ID>& message )
            {
                _signals[ID]( nullptr );
            }
        private:
            SignalMap   _signals;
            // TODO: Message Id Map
        };
    }
}

#endif