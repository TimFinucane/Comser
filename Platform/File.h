#pragma once

#include <string>

namespace FileSystem
{
    struct File
    {
        File( const std::wstring& filename );
        ~File();

        File( const File& file ) = delete;
        File& operator =( const File& file ) = delete;

        unsigned char*  file;
        unsigned int    length;
    };
}