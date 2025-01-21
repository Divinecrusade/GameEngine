#include "StarField.hpp"


StarField::StarField(HINSTANCE hInstance, int nCmdShow)
:
Game{ StarField::get_window(hInstance, nCmdShow), get_graphics() }
{ }

GameEngine::Interfaces::IWindow& StarField::get_window(HINSTANCE hInstance, int nCmdShow)
{
    static GameEngine::MainWindow wnd{ hInstance, nCmdShow, GAME_TITLE };
    return wnd;
}

GameEngine::Interfaces::IFramableGraphics2D& StarField::get_graphics()
{
    static GameEngine::GraphicsDirect2D gfx{ get_window().get_window_handler() };
    return gfx;
}
