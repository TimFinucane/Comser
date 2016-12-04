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
            typedef sigc::signal<void, const void*>         Signal;
            typedef std::map<MessageId, Signal>             SignalMap;
        public:
            /// <summary>
            /// Removes all signals.
            /// </summary>
            void                clear()
            {
                _signals.clear();
            }

            template <typename MESSAGE>
            void                clear()
            {
                clear( MESSAGE::id() );
            }
            void                clear( MessageId id )
            {
                _signals.erase( id );
            }

            template <typename MESSAGE>
            sigc::connection    connect( typename MESSAGE::Slot slot )
            {
                return _signals[MESSAGE::id()].connect( sigc::retype( slot ) );
            }
            
            template <MessageId ID, typename ARG>
            void                operator()( const message<ID, ARG>& message )
            {
                _signals[ID]( (void*)&message.arg );
            }
            template <MessageId ID>
            void                operator()( const message<ID>& message )
            {
                _signals[ID]( nullptr );
            }

            template <typename T>
            void                operator()( MessageId id, const T* t )
            {
                _signals[id]( (const void*)t );
            }
        private:
            SignalMap   _signals;
            // TODO: Message Id Map
        };
    }
}

#endif