#pragma once

#include <vector>
#include <memory>

namespace Graphics
{
    class _ObjectArrayBase
    {
    public:
        enum class UpdateFrequency
        {
            ONCE,
            OCCASIONALLY,
            FREQUENTLY
        };
    
    protected:
        _ObjectArrayBase( UpdateFrequency freq );
        _ObjectArrayBase( UpdateFrequency freq, size_t length, void* data );
        ~_ObjectArrayBase();

        void        recreate( size_t length, void* data );
        void        replace( size_t start, size_t length, void* data );
    private:
        UpdateFrequency _frequency;
        unsigned int    _buffer;
    };

    // An array object is a single buffer used by the
    //  graphics device to render objects
    template <typename OBJECT>
    class ObjectArray : protected _ObjectArrayBase
    {
        typedef std::vector<Object> Buffer;
    public:
        ObjectArray( UpdateFrequency freq )
            : _ObjectArrayBase( freq ), _length( 0 )
        {
        }
        ObjectArray( UpdateFrequency freq, Buffer buffer )
            : _ObjectArrayBase( freq, buffer.size() * sizeof( OBJECT ), &buffer[0] )
        {
        }
        ~ObjectArray()
        {
        }

        /// <summary>
        /// Updates the object array without
        ///  mapping it to the client side.
        /// </summary>
        void        update( Buffer buffer )
        {
            if( buffer.size() == _size )
                replace( 0, _size * sizeof( OBJECT ), &buffer[0] );
            else
                recreate( _size, &buffer[0] );
        }

        // TODO:
        /// <summary>
        /// Will push some updates to the object array
        /// </summary>
        //void        push( InstructionBuffer instructionBuffer );

    private:
        size_t  _size;
    };
}