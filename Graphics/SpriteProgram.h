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
    // TODO: Docs
    /// <summary>
    /// This
    /// </summary>
    class SpriteProgram : public Graphics::Shaders::Program
    {
    public:
        // TODO: Packing?
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
        SpriteProgram( const FileSystem::File& vertex, const FileSystem::File& fragment );
        ~SpriteProgram();

        // Draws all sprites
        void    draw( const Sprites& sprites, const TextureAtlas& atlas );
    private:
        void    defineBufferInfo();
        void    bindVaoState();

        unsigned int                            _vao;

        ObjectArray<Vertex>                     _vertices{ Buffer::UpdateFrequency::ONCE };
        ObjectArray<unsigned short>             _indices { Buffer::UpdateFrequency::ONCE };

        ObjectArray<Sprite>                     _sprites { Buffer::UpdateFrequency::FREQUENTLY };

        Graphics::Shaders::IndexedBufferInfo    _squareInfo;
        Graphics::Shaders::BufferInfo           _spriteInfo;
    };
}