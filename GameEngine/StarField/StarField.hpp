#pragma once

#include <Game.hpp>
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>
#include <WorldTransformer.hpp>
#include <CoordinateTransformer.hpp>
#include <FrameTimer.hpp>

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
    
    static constexpr float CAMERA_MOVE_SPEED{ 5.f };
    static constexpr float CAMERA_ZOOM_FACTOR{ 1.1f };
    static constexpr float CAMERA_UNZOOM_FACTOR{ 1.f / CAMERA_ZOOM_FACTOR };

public:

    StarField(HINSTANCE hInstance, int nCmdShow);

    void update() override;
    void render() override;

private:

    void move_camera_by(Vec2f delta_pos) noexcept;
    void zoom_camera() noexcept;
    void unzoom_camera() noexcept;

private:

    static GameEngine::Interfaces::IWindow& get_window(HINSTANCE hInstance = NULL, int nCmdShow = 0);
    static GameEngine::Interfaces::IFramableGraphics2D& get_graphics();

private:

    GameEngine::CoordinateTransformer ct;
    GameEngine::WorldTransformer wt{ };
    GameEngine::FrameTimer ft{ };

    std::vector<Star> stars{ };
    Rec2f cur_camera{ -WINDOW_WIDTH / 2.f, WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f, -WINDOW_HEIGHT / 2.f };
};
