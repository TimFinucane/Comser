#pragma once
#include <vector>

#include "Buffer.h"

namespace Graphics
{
    // An array object is a single buffer used by the
    //  graphics device to render objects
    template <typename OBJECT>
    class ObjectArray : public Buffer
    {
        typedef std::vector<OBJECT> Objects;
    public:
        ObjectArray( UpdateFrequency freq )
            : Buffer( freq ), _size( 0 ), _capacity( 0 )
        {
        }
        ObjectArray( UpdateFrequency freq, const Objects& buffer )
            : Buffer( freq, buffer.size() * sizeof( OBJECT ), &buffer[0] ), _size( buffer.size() ), _capacity( buffer.size() )
        {
        }
        ~ObjectArray()
        {
        }

        // <summary>
        // Updates the object array without mapping it to the client side.
        // </summary>
        void        update( const Objects& buffer )
        {
            _size = buffer.size();
            if( buffer.size() <= _capacity )
                replace( 0, _size * sizeof( OBJECT ), &buffer[0] );
            else
            {
                _capacity = buffer.size();
                recreate( _size * sizeof( OBJECT ), &buffer[0] );
            }
        }

        size_t      size() const
        {
            return _size;
        }

        // TODO:
        // <summary>
        // Will push some updates to the object array
        // </summary>
        //void        push( InstructionBuffer instructionBuffer );

    private:
        size_t  _size;
        size_t  _capacity;
    };


}