#pragma once

#include <Vector.h>
#include <Matrix.h>

#include <File.h>

#include "Program.h"
#include "BufferInfo.h"
#include "TextureAtlas.h"
#include "ObjectArray.h"

// Uses a 2D texture
namespace Graphics
{
    /// <summary>
    /// This
    class SpriteProgram : public Graphics::Shaders::Program
    {
    public:
        // TODO: Packing?
        struct Sprite
        {
            float           x;
            float           y;

            float           width;
            float           height;

            float           rotation;

            uint32_t        layer;
        };

        typedef ObjectArray<Sprite> Sprites;
    public:
        SpriteProgram( const FileSystem::File& vertex, const FileSystem::File& fragment );
        ~SpriteProgram();

        // Adds a sprite to the current rendering batch
        void    add( const Sprite& sprite );
        void    clearBatch();
        /// <summary>
        /// Changes a single sprite in the batch
        /// </summary>
        void    set( unsigned int index, const Sprite& sprite );

        // Draws all sprites
        void    draw( Sprites& sprites, const TextureAtlas& atlas );
    private:
        unsigned int    _vertexPos;
        unsigned int    _texCoordPos;
        unsigned int    _posUniform;
        unsigned int    _matUniform;

        Graphics::Shaders::BufferInfo   _array;
    };
}