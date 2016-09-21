#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "Encoding.h"

// TODO: Extensions?
// This filesystem works in effect by using globals in the .cpp file as an
//  alternative to a class ( which would have to very likely be a singleton )
namespace FileSystem
{
    typedef Encoding::AsciiText     Handle;

    struct StreamHandle;

    struct Metrics;
    struct Buffer
    {
        // TODO: Encoding types

        const Encoding::AsciiText   buffer;
        unsigned __int32            size;
    };

    void        init( Encoding::AsciiText rootLocation );
    void        release();

    Metrics     getFileMetrics( const Handle& handle );

    // Reads whole file into buffer
    Buffer      readFile( const Handle& handle );
    void        destroyFile( const Handle& handle );
}

#endif