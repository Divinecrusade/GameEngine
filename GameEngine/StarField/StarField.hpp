#pragma once

#include <Game.hpp>
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>
#include <WorldTransformer.hpp>
#include <CoordinateTransformer.hpp>
#include <FrameTimer.hpp>
#include <Camera.hpp>

#include <random>
#include <ranges>
#include <algorithm>

#include "Star.hpp"


class StarField : public GameEngine::Game
{
public:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;
    using Rec2f = GameEngine::Geometry::Rectangle2D<float>;

public:

    static constexpr int SCREEN_WIDTH{ 1920 };
    static constexpr int SCREEN_HEIGHT{ 1080 };


    static constexpr std::wstring_view GAME_TITLE{ L"StarField" };
    static constexpr int   WINDOW_WIDTH{ 800 };
    static constexpr int   WINDOW_HEIGHT{ WINDOW_WIDTH * 2 / 3 };
    static constexpr bool  WINDOW_RESIZABLE{ false };
    static constexpr Vec2i WINDOW_POS{ SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2 - 100 };
    
    static constexpr float CAMERA_MOVE_SPEED { 25.f };
    static constexpr float CAMERA_ZOOM_DELTA { 1.f / 40.f };
    static constexpr float CAMERA_MIN_ZOOM   { CAMERA_ZOOM_DELTA * 8.f };
    static constexpr float CAMERA_MAX_ZOOM   { 5.f };
    static constexpr GameEngine::Camera::radian CAMERA_ROTATION_DELTA{ static_cast<GameEngine::Camera::radian>(std::numbers::pi / 2.f) };
    static constexpr Rec2f CAMERA_AREA{ -WINDOW_WIDTH / 2.f, WINDOW_WIDTH / 2.f, -WINDOW_HEIGHT / 2.f, WINDOW_HEIGHT / 2.f };

    static constexpr float WORLD_WIDTH{ 10000.f };
    static constexpr float WORLD_HEIGHT{ WORLD_WIDTH / 3.f * 2.f };

    static constexpr float STAR_MEAN_RADIUS{ 50.f };
    static constexpr float STAR_DEV_RADIUS{ 10.f };

    static constexpr float STAR_MIN_RADIUS{ 35.0f };
    static constexpr float STAR_MAX_RADIUS{ 80.0f };

    static constexpr float STAR_MEAN_FLARES_COUNT{ 6.5f };
    static constexpr float STAR_DEV_FLARES_COUNT{ 2.f };

    static constexpr float STAR_MEAN_RADIUS_FACTOR{ 0.7f };
    static constexpr float STAR_DEV_RADIUS_FACTOR { 0.25f };

    static constexpr int STAR_MIN_FLARES_COUNT{ 4 };
    static constexpr int STAR_MAX_FLARES_COUNT{ 10 };

    static constexpr float STAR_MEAN_COLOUR_FREQUENCY{ 1.8f };
    static constexpr float STAR_DEV_COLOUR_FREQUENCY{ 1.f };

    static constexpr float STAR_MIN_COLOUR_FREQUENCY{ 0.6f };
    static constexpr float STAR_MAX_COLOUR_FREQUENCY{ 4.0f };

    static constexpr float STAR_MAX_RADIUS_FACTOR{ 0.95f };
    static constexpr float STAR_MIN_RADIUS_FACTOR{ 0.45f };

    static constexpr float STAR_MIN_ROTATION_SPEED{ -1.f * static_cast<float>(std::numbers::pi) };
    static constexpr float STAR_MAX_ROTATION_SPEED{ 1.f  * static_cast<float>(std::numbers::pi) };

    static constexpr std::size_t STARS_TOTAL_COUNT{ 1000U };

public:

    StarField(HINSTANCE hInstance, int nCmdShow);

    void update() override;
    void render() override;

private:

    static GameEngine::Interfaces::IWindow& get_window(HINSTANCE hInstance = NULL, int nCmdShow = 0);
    static GameEngine::Interfaces::IFramableGraphics2D& get_graphics();

private:

    GameEngine::CoordinateTransformer ct;
    GameEngine::WorldTransformer wt{ };
    GameEngine::FrameTimer ft{ };

    std::vector<Star> stars{ };
    GameEngine::Camera cam;
};
