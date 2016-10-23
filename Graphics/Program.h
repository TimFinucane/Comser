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
            Program( std::initializer_list<Shader> shaders );
            ~Program();

            Program( Program&& tex ) = default;
            Program& operator =( Program&& tex ) = default;

            void    use();
            void    bindVao();
        protected:

        private:
            unsigned int    _vao;
            unsigned int    _program;
        };
    }
}