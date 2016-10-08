#include "Image.h"

#include <GraphicsIncludes.h>

#include <png.h>

using namespace Graphics::Image;

const unsigned int SIGNATURE_SIZE = 8;

void readData( png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead );

File    File::createPng( std::istream& png )
{
    // Read the signature and check that it's good
    unsigned char signature[SIGNATURE_SIZE];

    png.read( (char*)signature, SIGNATURE_SIZE );

    if( !png_check_sig( signature, SIGNATURE_SIZE ) )
        return File();

    png_structp png_ptr = 0;
    png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr );

    if( !png_ptr )
        return File();

    png_infop info_ptr = 0;
    info_ptr = png_create_info_struct( png_ptr );

    if( !info_ptr )
    {
        png_destroy_read_struct( &png_ptr, nullptr, nullptr );
        return File();
    }

    png_set_read_fn( png_ptr, &png, readData );

    // and lets tell libpng we already read the signature
    png_set_sig_bytes( png_ptr, SIGNATURE_SIZE );

    // Read the picture info
    png_read_info( png_ptr, info_ptr );

    png_uint_32 w = 0;
    png_uint_32 h = 0;
    int bitDepth = 0;
    int colourType = -1;
    unsigned int channels = 0;
    png_uint_32 retval = png_get_IHDR( png_ptr, info_ptr,
                                       &w,
                                       &h,
                                       &bitDepth,
                                       &colourType,
                                       nullptr, nullptr, nullptr );

    if( retval != 1 )
    {
        png_destroy_read_struct( &png_ptr, &info_ptr, nullptr );
        return File();
    }

    switch( colourType )
    {
    case PNG_COLOR_TYPE_GRAY:
        channels = 1;
        break;
    case PNG_COLOR_TYPE_RGB:
        channels = 3;
        break;
    case PNG_COLOR_TYPE_RGBA:
        channels = 4;
        break;
    default:
        channels = 0;
        throw;
        break;
    }

    File image( w, h, bitDepth, channels );

    // And now read it for file copying
    size_t rowbytes = png_get_rowbytes( png_ptr, info_ptr );

    // And manipulate it so that it works
    // TODO: Currently don't know what to do with pixel packing
    unsigned char** rowPointers = new unsigned char*[h];
    for( unsigned int i = 0; i < h; i++ )
    {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order is swapped
        rowPointers[i] = image.file() + (rowbytes * (h - (i + 1)));
    }

    png_read_image( png_ptr, rowPointers );
    
    png_destroy_read_struct( &png_ptr, &info_ptr, nullptr );

    return image;
}

void readData( png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead )
{
    png_voidp io_ptr = png_get_io_ptr( png_ptr );
    /*if( io_ptr == nullptr )
    return;   // add custom error handling here*/

    // Get the stream
    std::istream& file = *(std::istream*)io_ptr;
    // And perform the read operation
    file.read( (char*)outBytes, (size_t)byteCountToRead );
}