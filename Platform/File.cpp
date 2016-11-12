#include "File.h"

#include <fstream>

using namespace FileSystem;

File::File( const std::wstring& filename )
    : File( filename.c_str() )
{
}
File::File( const wchar_t filename[] )
{
    std::ifstream f( filename, std::fstream::binary | std::fstream::ate );
    length = f.tellg();
    f.seekg( std::ios::beg, 0 );

    file = new unsigned char[length];
    f.read( reinterpret_cast<char*>(file), length );
}
File::~File()
{
    delete[] file;
}