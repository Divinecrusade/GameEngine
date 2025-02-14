#include "StarField.hpp"


StarField::StarField(HINSTANCE hInstance, int nCmdShow)
:
Game{ StarField::get_window(hInstance, nCmdShow), get_graphics() },
ct{ Rec2i{ Vec2i{ 0, 0 }, gfx.get_screen_width(), gfx.get_screen_height() } },
cam{ CAMERA_AREA, wt, CAMERA_MIN_ZOOM, CAMERA_MAX_ZOOM }
{ 
    std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> x_dist{ -WORLD_WIDTH / 2.0f, WORLD_WIDTH / 2.0f };
    std::uniform_real_distribution<float> y_dist{ -WORLD_HEIGHT / 2.0f, WORLD_HEIGHT / 2.0f };
    std::normal_distribution<float> radius_dist { STAR_MEAN_RADIUS, STAR_DEV_RADIUS };
    std::normal_distribution<float> flare_dist  { STAR_MEAN_FLARES_COUNT, STAR_DEV_FLARES_COUNT };

    constexpr std::array<GameEngine::Colour, 6U> colours_pull
    { 
        GameEngine::Colours::RED,
        GameEngine::Colours::WHITE,
        GameEngine::Colours::BLUE, 
        GameEngine::Colours::CYAN,
        GameEngine::Colours::MAGENTA,
        GameEngine::Colours::GREEN 
    };
    std::uniform_int_distribution<std::size_t> colour_sampler{ 0U, colours_pull.size() - 1U };

    std::normal_distribution<float>       colour_frequency_dist{ STAR_MEAN_COLOUR_FREQUENCY, STAR_DEV_COLOUR_FREQUENCY };
    std::uniform_real_distribution<float> colour_phase_dist    { 0.0f, 2.f * static_cast<float>(std::numbers::pi) };
    std::normal_distribution<float>       radius_factor_dist   { STAR_MEAN_RADIUS_FACTOR, STAR_DEV_RADIUS_FACTOR };
    std::uniform_real_distribution<float> rotation_speed_dist  { STAR_MIN_ROTATION_SPEED, STAR_MAX_ROTATION_SPEED };

    while (stars.size() != STARS_TOTAL_COUNT)
    {
        const float radius{ std::clamp(radius_dist(rng), STAR_MIN_RADIUS, STAR_MAX_RADIUS) };
        const Vec2f pos{ x_dist(rng), y_dist(rng) };
        if (std::ranges::any_of(stars, [pos](auto const& star)
            { return (star.get_pos() - pos).get_length() < STAR_MAX_RADIUS * std::sqrtf(3.f); }))
        {
            continue;
        }

        const GameEngine::Colour c{ colours_pull[colour_sampler(rng)] };
        const int flares_count{ std::clamp((int)flare_dist(rng), STAR_MIN_FLARES_COUNT, STAR_MAX_FLARES_COUNT) };
        const float colour_freq{ std::clamp(colour_frequency_dist(rng), STAR_MIN_COLOUR_FREQUENCY, STAR_MAX_COLOUR_FREQUENCY) };
        const float colour_phase{ colour_phase_dist(rng) };
        float const radius_factor{ std::clamp(radius_factor_dist(rng), STAR_MIN_RADIUS_FACTOR, STAR_MAX_RADIUS_FACTOR) };
        float const rotation_speed{ std::clamp(rotation_speed_dist(rng), STAR_MIN_ROTATION_SPEED, STAR_MAX_ROTATION_SPEED) };
        stars.emplace_back(pos, radius, flares_count, c, rotation_speed, radius_factor, colour_freq, colour_phase);
    }
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
