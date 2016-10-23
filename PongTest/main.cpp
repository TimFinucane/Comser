#include <Window.h>
#include <SpriteProgram.h>

#include <Image.h>

#include <memory>

void loop();
bool event( SDL_Event e );

std::shared_ptr<Graphics::SpriteProgram> spriteProgram;
std::shared_ptr<Graphics::TextureAtlas> textureAtlas;
std::shared_ptr<Window> window;

int main( int argc, char *argv[] )
{
    (argc);
    (argv);
    try
    {
    window = std::make_shared<Window>(
        Window::WindowSettings{
            "Window",
            Window::Mode::WINDOWED,
            Window::defaultScreen(),
            Window::Rect{ 100, 100, 800, 600 }
        } );
    
    FileSystem::File vert( L"SpriteVertex.sh" );
    FileSystem::File frag( L"SpriteFragment.sh" );

    spriteProgram = std::make_shared<Graphics::SpriteProgram>( vert, frag );

    Graphics::Image img = Graphics::Image::createPng( FileSystem::File( L"tilemap.png" ) );

    textureAtlas = std::make_shared<Graphics::TextureAtlas>( img, 32, 32, 2 );

    window->loop( sigc::ptr_fun( loop ), sigc::ptr_fun( event ) );
    }
    catch( const std::exception& e )
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Runtime error", e.what(), nullptr );
    }
    textureAtlas.reset();
    spriteProgram.reset();
    window.reset();

    return 0;
}

bool event( SDL_Event e )
{
    if( e.type == SDL_WINDOWEVENT && e.window.type == SDL_WINDOWEVENT_CLOSE || e.type == SDL_QUIT )
        return false;
    return true;
}

void loop()
{
    window->clear( 0.0f, 0.0f, 1.0f, 1.0f );

    Graphics::SpriteProgram::Sprites sprites;
    sprites.push_back( { 0.0f, 0.0f, 1.0f, 1.0f, 0 } );

    spriteProgram->draw( sprites, *textureAtlas );

    window->update();
}