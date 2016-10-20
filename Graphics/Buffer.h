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
    protected:
        Buffer( UpdateFrequency freq );
        Buffer( UpdateFrequency freq, size_t length, void* data );
        ~Buffer();

        void        recreate( size_t length, void* data );
        void        replace( size_t start, size_t length, void* data );
    private:
        UpdateFrequency _frequency;
        unsigned int    _buffer;
    };
}