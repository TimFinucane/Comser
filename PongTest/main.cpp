#include <Window.h>
#include <Atlas2DProgram.h>

int main()
{
    Window window( 
    {
        "Window",
        Window::Mode::WINDOWED,
        Window::defaultScreen(),
        Window::Rect{ 100, 100, 800, 600 }
    } );
    
    
   
}