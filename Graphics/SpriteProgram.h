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
    /// This class is used for rendering a series of 2d sprites to the screen.
    /// To use this class, you must have a texture atlas that holds all the desired sprites.
    /// </summary>
    class SpriteProgram : public Graphics::Shaders::Program
    {
    public:
        // TODO: Rotation
        /// <summary>
        /// This struct represents a single sprite being rendered onto the
        ///  screen.
        /// </summary>
        struct Sprite
        {
            float           x;
            float           y;

            float           xScale;
            float           yScale;

            uint32_t        layer;
        };
        typedef std::vector<Sprite> Sprites;

    private:
        struct Vertex
        {
            float xPos;
            float yPos;

            float xTex;
            float yTex;
        };
    public:
        /// <summary>
        /// Initialises the sprite program with the necessary shader files.
        /// For this class to work, the files must be SpriteFragment.sh and SpriteVertex.sh
        /// </summary>
        SpriteProgram( const FileSystem::File& vertex, const FileSystem::File& fragment );
        ~SpriteProgram();

        /// <summary>
        /// Draws all the given sprites using the given texture atlas.
        /// If you want to use a different texture atlas then you will need to
        ///  call draw again with that new atlas.
        /// </summary>
        void    draw( const Sprites& sprites, const TextureAtlas& atlas );
    private:
        // Called in constructor for filling BufferInfo data
        void    defineBufferInfo();
        // Called in constructor for setting up vertex attributes
        void    bindVaoState();

        ObjectArray<Vertex>                     _vertices{ Buffer::UpdateFrequency::ONCE };
        ObjectArray<unsigned short>             _indices { Buffer::UpdateFrequency::ONCE };

        ObjectArray<Sprite>                     _sprites { Buffer::UpdateFrequency::FREQUENTLY };

        Graphics::Shaders::IndexedBufferInfo    _squareInfo;
        Graphics::Shaders::BufferInfo           _spriteInfo;
    };
}