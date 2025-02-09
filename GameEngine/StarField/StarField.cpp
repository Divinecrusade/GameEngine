#include "StarField.hpp"


StarField::StarField(HINSTANCE hInstance, int nCmdShow)
:
Game{ StarField::get_window(hInstance, nCmdShow), get_graphics() },
ct{ Rec2i{ Vec2i{ 0, 0 }, gfx.get_screen_width(), gfx.get_screen_height() } }
{ 
    stars.emplace_back(Vec2f{ 0.f, 0.f }, 100.f, 6, GameEngine::Colours::RED, 0.25f, 0.5f, 1.0f, static_cast<float>(std::numbers::pi / 2.));
    stars.emplace_back(Vec2f{ 100.f, -100.f }, 50.f, 5, GameEngine::Colours::YELLOW, 0.5f, 0.75f, 1.2f, static_cast<float>(std::numbers::pi * 3. / 2.));
    stars.emplace_back(Vec2f{ -100.f, -100.f }, 25.f, 4, GameEngine::Colours::GREEN, 1.f, 0.9f, 1.8f, static_cast<float>(std::numbers::pi));
}

void StarField::update()
{
    float const dt{ ft.mark() };
    for (auto& star : stars)
    {
        star.update(dt);
    }

    if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_LEFT))
    {
        move_camera_by({ -CAMERA_MOVE_SPEED, 0.f });
    }
    else if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_RIGHT))
    {
        move_camera_by({ CAMERA_MOVE_SPEED, 0.f });
    }
    else if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_DOWN))
    {
        move_camera_by({ 0.f, -CAMERA_MOVE_SPEED });
    }
    else if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_UP))
    {
        move_camera_by({ 0.f, CAMERA_MOVE_SPEED });
    }
}

void StarField::render()
{
    for (auto& star : stars | std::views::filter
        (
            [camera = cur_camera](auto const& model)
            { 
                auto const model_hitbox{ model.get_square() };
                return camera.is_colided_with(model_hitbox) || camera.contains(model_hitbox);
            }
        )
    )
    {
        gfx.draw_polygon(ct.transform(wt.transform(star.get_shape())), star.STROKE_WIDTH, star.get_colour());
    }
}

void StarField::move_camera_by(Vec2f delta_pos) noexcept
{
    wt.translate(delta_pos * -1.f);
    cur_camera.left   += delta_pos.x;
    cur_camera.right  += delta_pos.x;
    cur_camera.bottom += delta_pos.y;
    cur_camera.top    += delta_pos.y;
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
