#include "StarField.hpp"


StarField::StarField(HINSTANCE hInstance, int nCmdShow)
:
Game{ StarField::get_window(hInstance, nCmdShow), get_graphics() },
ct{ Rec2i{ Vec2i{ 0, 0 }, gfx.get_screen_width(), gfx.get_screen_height() } },
cam{ CAMERA_AREA, wt, CAMERA_MIN_ZOOM, CAMERA_MAX_ZOOM }
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
        cam.move({ -CAMERA_MOVE_SPEED, 0.f });
    }
    if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_RIGHT))
    {
        cam.move({ CAMERA_MOVE_SPEED, 0.f });
    }
    if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_DOWN))
    {
        cam.move({ 0.f, -CAMERA_MOVE_SPEED });
    }
    if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_UP))
    {
        cam.move({ 0.f, CAMERA_MOVE_SPEED });
    }

    if (get_wnd().is_non_fun_key_pressed('Z'))
    {
        cam.set_zoom(cam.get_zoom() + CAMERA_ZOOM_DELTA);
    }
    else if (get_wnd().is_non_fun_key_pressed('X'))
    {
        cam.set_zoom(cam.get_zoom() - CAMERA_ZOOM_DELTA);
    }

    if (get_wnd().is_non_fun_key_pressed('Q'))
    {
        cam.rotate(CAMERA_ROTATION_DELTA * dt);
    }
    else if (get_wnd().is_non_fun_key_pressed('E'))
    {
        cam.rotate(-CAMERA_ROTATION_DELTA * dt);
    }
}

void StarField::render()
{
    for (auto& star : stars)
    {
        GameEngine::Shape model{ star.get_shape() };
        if (cam.contains(model))
        {
            gfx.draw_polygon(ct.transform(wt.transform(std::move(model))), star.STROKE_WIDTH, star.get_colour());
        }
    }

    GameEngine::Shape tmp{ std::vector<Vec2f>{ {cam.get_area().left, cam.get_area().top}, {cam.get_area().right, cam.get_area().bottom} }};
    auto verts{ ct.transform(std::move(tmp)) };
    gfx.draw_rectangle({ verts.front().x, verts.back().x, verts.back().y, verts.front().y}, 1, GameEngine::Colours::RED);
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
