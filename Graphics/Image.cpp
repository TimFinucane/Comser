#include "Image.h"

#include <GraphicsIncludes.h>

#include <png.h>
#include "lodepng.h"

using namespace Graphics;

const unsigned int SIGNATURE_SIZE = 8;

struct FileInfo
{
    FileInfo( const FileSystem::File& f )
        : file( f ), curPt( 0 )
    {
    }

    const FileSystem::File& file;
    unsigned int curPt;
};

Image Image::createPng( const FileSystem::File& png )
{
    std::vector<unsigned char> image;
    unsigned int width;
    unsigned int height;
    unsigned int state = lodepng::decode( image, width, height, png.file, png.length );

    if( state != 0 )
        throw std::runtime_error( std::string( "LodePng error: " ) + lodepng_error_text( state ) );
    
    return Image( width, height, 8, 4, &image[0] );
}