#pragma once

#include <string>

namespace FileSystem
{
    struct File
    {
        // TODO: Need to check if linking is required for this constructor by Graphics.lib if it is
        //  never used
        File( const std::wstring& filename );
        ~File();

        File( const File& file ) = delete;
        File& operator =( const File& file ) = delete;

        unsigned char*  file;
        unsigned int    length;
    };
}