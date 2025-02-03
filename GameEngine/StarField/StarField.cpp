#include "StarField.hpp"


StarField::StarField(HINSTANCE hInstance, int nCmdShow)
:
Game{ StarField::get_window(hInstance, nCmdShow), get_graphics() },
ct{ Rec2i{ Vec2i{ 0, 0 }, gfx.get_screen_width(), gfx.get_screen_height() } }
{ 
    stars.emplace_back(Vec2f{ 0.f, 0.f }, 100.f, 6, GameEngine::Colours::RED, 0.25f, 0.5f);
    stars.emplace_back(Vec2f{ 100.f, -100.f }, 50.f, 5, GameEngine::Colours::YELLOW, 0.5f, 0.75f);
    stars.emplace_back(Vec2f{ -100.f, -100.f }, 25.f, 4, GameEngine::Colours::GREEN, 1.f, 0.9f);
}

void StarField::update()
{
    float const dt{ ft.mark() };
    for (auto& star : stars)
    {
        star.update(dt);
    }
}

void StarField::render()
{
    for (auto& star : stars)
    {
        gfx.draw_polygon(ct.transform(star.get_shape()), star.STROKE_WIDTH, star.get_colour());
    }
    gfx.draw_ellipse({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, 100, 100, 2, GameEngine::Colours::RED);
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
