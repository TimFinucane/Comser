#pragma once

#include <string>

#include <File.h>

namespace Graphics
{
    namespace Shaders
    {
        class Shader
        {
            friend class Program;
        public:
            // TODO: throwing
            Shader( unsigned int shaderType, const FileSystem::File& file );
            ~Shader();

            Shader( Shader&& shader ) noexcept = default;
            Shader& operator =( Shader&& shader ) noexcept = default;
        private:
            unsigned int _shader;
        };

    }
}