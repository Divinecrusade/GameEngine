#pragma once

#include <Game.hpp>
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>


class StarField : public GameEngine::Game
{
public:

    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr int SCREEN_WIDTH{ 1920 };
    static constexpr int SCREEN_HEIGHT{ 1080 };


    static constexpr std::wstring_view GAME_TITLE{ L"StarField" };
    static constexpr int   WINDOW_WIDTH{ 800 };
    static constexpr int   WINDOW_HEIGHT{ WINDOW_WIDTH * 2 / 3 };
    static constexpr bool  WINDOW_RESIZABLE{ false };
    static constexpr Vec2i WINDOW_POS{ SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2 - 100 };

public:

    StarField(HINSTANCE hInstance, int nCmdShow);

private:

    static GameEngine::Interfaces::IWindow& get_window(HINSTANCE hInstance = NULL, int nCmdShow = 0);
    static GameEngine::Interfaces::IFramableGraphics2D& get_graphics();

private:

};
