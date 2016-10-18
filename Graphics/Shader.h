#pragma once

#include <string>

namespace Graphics
{
    namespace Shaders
    {
        class Shader
        {
            friend class Program;
        public:
            Shader( unsigned int shaderType, const char* file, int length, std::string& error );
            ~Shader();

            Shader( Shader&& shader ) noexcept = default;
            Shader& operator =( Shader&& shader ) noexcept = default;
        private:
            unsigned int _shader;
        };

    }
}