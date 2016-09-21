#include "Library.h"

#include <freetype.h>
#include <exception>

#define CHECK_ERROR(error, text) if( error ) throw std::runtime_error( text );

using namespace Graphics::Text;

void    Library::init()
{
    FT_Error error = FT_Init_FreeType( &_library );
    CHECK_ERROR( error, "Freetype library couldnt be created" )
}
void    Library::release()
{
    FT_Done_FreeType( _library );
}

