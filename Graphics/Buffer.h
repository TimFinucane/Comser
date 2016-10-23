#pragma once

namespace Graphics
{
    class Buffer
    {
    public:
        enum class UpdateFrequency
        {
            ONCE,
            OCCASIONALLY,
            FREQUENTLY
        };

        unsigned int    buffer()
        {
            return _buffer;
        }
        /// <summary>
        /// Binds the buffer to the given buffertype.
        /// The default is the GL_ARRAY_BUFFER value
        /// </summary>
        void            bind( unsigned int bufferType = 0x8892 );
    protected:
        Buffer( UpdateFrequency freq );
        Buffer( UpdateFrequency freq, size_t length, const void* data );
        ~Buffer();

        void        recreate( size_t length, const void* data );
        void        replace( size_t start, size_t length, const void* data );
    private:
        UpdateFrequency _frequency;
        unsigned int    _buffer;
    };
}