#include <CppUnitTest.h>


#include <Device.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(GraphicsTest)
{
public:
    TEST_CLASS_INITIALIZE(initialize)
    {
        // Give it some general default setups. We aren't testing the Window class anyway.
        Window::WindowSettings settings;
        settings.mode = Window::Mode::WINDOWED;
        settings.rect = Window::Rect{ 100, 100, 800, 600 };
    }

private:
    Window              window;
    Graphics::Device    device;
};