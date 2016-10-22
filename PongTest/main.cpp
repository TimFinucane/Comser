#include <Window.h>
#include <SpriteProgram.h>

#include <Image.h>

#include <memory>

void loop();
bool event( SDL_Event e );

std::unique_ptr<Graphics::SpriteProgram> spriteProgram;
std::unique_ptr<Graphics::TextureAtlas> textureAtlas;

int main()
{
    Window window( 
    {
        "Window",
        Window::Mode::WINDOWED,
        Window::defaultScreen(),
        Window::Rect{ 100, 100, 800, 600 }
    } );
    
    window.loop( sigc::ptr_fun( loop ), sigc::ptr_fun( event ) );

    FileSystem::File vert( L"2DVertex.sh" );
    FileSystem::File frag( L"2DFragment.sh" );

    spriteProgram = std::make_unique<Graphics::SpriteProgram>( vert, frag );

    Graphics::Image img = Graphics::Image::createPng( FileSystem::File( L"tilemap.png" ) );

    textureAtlas = std::make_unique<Graphics::TextureAtlas>( img, 32, 32, 2 );
}

bool event( SDL_Event e )
{
    if( e.type == SDL_WINDOWEVENT && e.window.type == SDL_WINDOWEVENT_CLOSE )
        return true;
}

void loop()
{
    Graphics::SpriteProgram::Sprites sprites;
    sprites.push_back( { 400.0f, 300.0f, 20.0f, 20.0f, 0 } );

    spriteProgram->draw( sprites, *textureAtlas );
}