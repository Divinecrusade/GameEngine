#pragma once

#include <Game.hpp>
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>
#include <Rectangle2D.hpp>
#include <Mouse.hpp>

#include "ColorField.hpp"


class PaintItGit final : private GameEngine::Game
{
private:

    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

    static constexpr Rec2i WINDOW   { 0, 1200, 800, 0 };
    static constexpr bool  RESIZABLE{ false };

    static constexpr std::wstring_view GAME_TITLE{ L"Paint it Git" };

    static constexpr Rec2i PADDING{ 20, 20, 20, 20 };

    static constexpr int   COLOR_FIELD_SIZE{ WINDOW.get_height() - PADDING.top - PADDING.bottom };
    static constexpr Rec2i COLOR_FIELD_AREA{ PADDING.left, PADDING.left + COLOR_FIELD_SIZE, PADDING.top + COLOR_FIELD_SIZE, PADDING.top };
    static constexpr int   N_BLOCKS_IN_ROW{ 10 };

    static constexpr std::size_t N_COLORS{ 5 };

    static constexpr std::array<GameEngine::Colour, N_COLORS> MAIN_COLOURS
    { 
        GameEngine::Colours::RED,
        GameEngine::Colours::GREEN, 
        GameEngine::Colours::YELLOW,  
        GameEngine::Colours::BLUE,
        GameEngine::Colours::WHITE
    };

    static constexpr int MINIATURE_SIZE{ ColorField < COLOR_FIELD_SIZE, N_BLOCKS_IN_ROW, Vec2i{ COLOR_FIELD_AREA.left, COLOR_FIELD_AREA.top } >::BLOCK_SIZE / 2 };
    static constexpr int MINIATURE_STROKE_WIDTH{ 1 };
    static constexpr GameEngine::Colour MINIATURE_C{ GameEngine::Colours::BLACK };

private:

    enum class GameStage
    {
        INIT_COMMIT, COMITTING
    };

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

    void update_init_commit();

private:

    GameStage cur_stage{ GameStage::INIT_COMMIT };
    ColorField<COLOR_FIELD_SIZE, N_BLOCKS_IN_ROW, Vec2i{ COLOR_FIELD_AREA.left, COLOR_FIELD_AREA.top }> blocks{ MAIN_COLOURS };

    bool  hovered{ false };
    Vec2i cursor_pos{ };
    std::size_t cur_color_index{ 0U };

};