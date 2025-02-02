#include "StarField.hpp"


StarField::StarField(HINSTANCE hInstance, int nCmdShow)
:
Game{ StarField::get_window(hInstance, nCmdShow), get_graphics() },
ct{ Rec2i{ Vec2i{ 0, 0 }, gfx.get_screen_width(), gfx.get_screen_height() } }
{ }

void StarField::render()
{
    Star const test1{ { 0.f, 0.f }, 100.f, 5, GameEngine::Colours::RED };
    Star const test2{ { 100.f, -100.f }, 50.f, 5, GameEngine::Colours::YELLOW };
    Star const test3{ { -100.f, -100.f }, 25.f, 5, GameEngine::Colours::GREEN };
    gfx.draw_polygon(ct.transform(test1.get_shape()), test1.STROKE_WIDTH, test1.get_colour());
    gfx.draw_polygon(ct.transform(test2.get_shape()), test2.STROKE_WIDTH, test2.get_colour());
    gfx.draw_polygon(ct.transform(test3.get_shape()), test3.STROKE_WIDTH, test3.get_colour());
}

GameEngine::Interfaces::IWindow& StarField::get_window(HINSTANCE hInstance, int nCmdShow)
{
    static GameEngine::MainWindow wnd{ hInstance, nCmdShow, GAME_TITLE, WINDOW_RESIZABLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_POS.x, WINDOW_POS.y };
    return wnd;
}

GameEngine::Interfaces::IFramableGraphics2D& StarField::get_graphics()
{
    static GameEngine::GraphicsDirect2D gfx{ get_window().get_window_handler() };
    return gfx;
}
