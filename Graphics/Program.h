#pragma once

#include <istream>
#include <vector>

#include "Shader.h"

namespace Graphics
{
    namespace Shaders
    {
        /*              Program
         * A program is a complete set of shaders
         *  that defines a rendering path. This class
         *  is meant to be inherited from to support a
         *  specific set of shaders, to provide an
         *  interface between them and the program.
         */
        class Program
        {
        public:
            Program();
            ~Program()
            {
                destroy();
            }

            Program( Program&& tex ) = default;
            Program& operator =( Program&& tex ) = default;

            void    destroy();

            void    use();
        protected:
            void    create( const Shader* shaders, unsigned int count, std::string& error );

            unsigned int    _program;
        };
    }
}