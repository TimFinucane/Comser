#pragma once

#include <File.h>

namespace Graphics
{
    struct Image
    {
        Image()
            : _width( 0 ), _height( 0 ), _file( nullptr )
        {
        }
        Image( unsigned int width, unsigned int height, unsigned int bitDepth, unsigned int channels )
            : _width( width ), _height( height ), _bitDepth( bitDepth ), _channels( channels )
        {
            _file = new unsigned char[size()];
        }

        Image::~Image()
        {
            if( _file != nullptr )
                delete[] _file;
        }

        static Image createPng( const FileSystem::File& file );

        // No copying allowed as this contains
        //  dynamically allocated data
        Image( Image& ) = delete;
        Image& operator =( Image& ) = delete;

        Image( Image&& info ) noexcept = default; // TODO: set _file nullptr first
        Image& operator= ( Image&& info ) noexcept = default;

        unsigned int                size() const
        {
            return _width * _height * pixelDepth();
        }

        unsigned int                pixelDepth() const
        {
            // Produces a ceiling value for the bits per pixel
            return (unsigned int)((_bitDepth * _channels - 1) / 8.0) + 1;
        }

        unsigned int                channels() const
        {
            return _channels;
        }
        // Depth of a single channel
        unsigned int                bitDepth() const
        {
            return _bitDepth;
        }

        unsigned int                width() const
        {
            return _width;
        }
        unsigned int                height() const
        {
            return _height;
        }

        unsigned char* const        file()
        {
            return _file;
        }
        const unsigned char* const  file() const
        {
            return _file;
        }

        // The start position of the given pixel
        unsigned char*              file( unsigned int x, unsigned int y )
        {
            return &_file[y * _width + x];
        }
        const unsigned char*        file( unsigned int x, unsigned int y ) const
        {
            return &_file[y * _width + x];
        }
    private:

        unsigned int    _bitDepth; // Size of a single colour channel in bits
        unsigned int    _channels; // Number of channels

        unsigned int    _width;
        unsigned int    _height;

        unsigned char*  _file;
    };
}