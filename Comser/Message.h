#pragma once

#include <sigc++/sigc++.h>

namespace Comser::Event
{
    typedef uint64_t MessageId;

    // Message capable of holding 0 or 1 arguments.
    template<MessageId ID, typename... ARGS>
    struct Message
    {
    };

    template<MessageId ID, typename ARG>
    struct Message<ID, ARG>
    {
    public:
        typedef sigc::slot<void, ARG> Slot;

        static constexpr MessageId id()
        {
            return ID;
        }
    public:
        Message( const ARG& arg )
            : arg( arg )
        {
        }

        const ARG&      arg;
    };

    template<MessageId ID>
    struct Message<ID>
    {
    public:
        typedef sigc::slot<void>    Slot;

        static constexpr MessageId id()
        {
            return ID;
        }
    public:
        Message()
        {
        }
    };

    // Hash function for converting strings to numbers.
    // Currently very simple, might upgrade if necessary
    constexpr MessageId hash( const char* s, unsigned int n = 0 )
    {
        return (*s == 0 || n >= 8) ? 0 :
            ((int64_t)s[0] << (n * 8)) + hash( s + 1, n + 1 );
    }

    template <MessageId ID>
    struct _construct
    {
        template<typename ...ARGS>
        using Message = Message<ID, ARGS...>;
    };

    // This is an easy way of constructing a message
    // You can just go construct( "MessageName" )::Message<ARGS>
    #define construct( str ) _construct<hash(str)>

}