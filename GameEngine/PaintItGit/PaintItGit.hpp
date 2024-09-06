#pragma once

#include <Game.hpp>
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>
#include <Rectangle2D.hpp>


class PaintItGit final : private GameEngine::Game
{
private:

    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

    static constexpr Rec2i WINDOW   { 0, 1200, 800, 0 };
    static constexpr bool  RESIZABLE{ false };

    static constexpr std::wstring_view GAME_TITLE{ L"Paint it Git" };

public:

    static PaintItGit& get_game(HINSTANCE hInstance, int nCmdShow);
    static PaintItGit& get_game();

private:

    PaintItGit(GameEngine::MainWindow& window, GameEngine::GraphicsDirect2D& graphics);
    
public:

    PaintItGit() = delete;
    PaintItGit(PaintItGit const&) = delete;
    PaintItGit(PaintItGit&&)      = delete;

    PaintItGit& operator=(PaintItGit const&) = delete;
    PaintItGit& operator=(PaintItGit&&)      = delete;

    virtual ~PaintItGit() = default;

    using Game::start;

private:

    virtual void update();
    virtual void render();
};