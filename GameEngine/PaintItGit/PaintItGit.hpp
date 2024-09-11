#pragma once

#include <Game.hpp>
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>
#include <Rectangle2D.hpp>
#include <Mouse.hpp>
#include <FrameTimer.hpp>

#include "ColourField.hpp"
#include "PulsationEffect.hpp"


class PaintItGit final : private GameEngine::Game
{
private:

    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

    static constexpr Rec2i WINDOW   { 0, 1000, 600, 0 };
    static constexpr bool  RESIZABLE{ false };

    static constexpr std::wstring_view GAME_TITLE{ L"Paint it Git" };

    static constexpr Rec2i PADDING{ 20, 20, 20, 20 };

    static constexpr int   COLOUR_FIELD_SIZE{ WINDOW.get_height() - PADDING.top - PADDING.bottom };
    static constexpr Rec2i COLOUR_FIELD_AREA{ PADDING.left, PADDING.left + COLOUR_FIELD_SIZE, PADDING.top + COLOUR_FIELD_SIZE, PADDING.top };
    static constexpr int   N_BLOCKS_IN_ROW{ 10 };

    static constexpr std::size_t N_COLOURS{ 5 };

    static constexpr std::array<GameEngine::Colour, N_COLOURS> MAIN_COLOURS
    { 
        GameEngine::Colours::RED,
        GameEngine::Colours::GREEN, 
        GameEngine::Colours::YELLOW,  
        GameEngine::Colours::BLUE,
        GameEngine::Colours::WHITE
    };

    static constexpr int MINIATURE_SIZE{ ColourField < COLOUR_FIELD_SIZE, N_BLOCKS_IN_ROW, Vec2i{ COLOUR_FIELD_AREA.left, COLOUR_FIELD_AREA.top } >::BLOCK_SIZE / 2 };
    static constexpr int MINIATURE_STROKE_WIDTH{ 1 };

    static constexpr GameEngine::Colour INNER_BORDER_C{ GameEngine::Colours::ROYAL_BLUE };
    static constexpr int INNER_BORDER_THICKNESS{ 2 };
    static constexpr GameEngine::Colour OUTER_BORDER_C{ GameEngine::Colours::CYAN };
    static constexpr int OUTER_BORDER_THICKNESS{ INNER_BORDER_THICKNESS };

    static constexpr GameEngine::Colour BACKGROUND_COLOUR{ GameEngine::Colours::BLACK };
    static constexpr int MAX_BLOCK_TRANSPERENCY{ 70 };

private:

    enum class GameStage
    {
        INIT_COMMIT, COMMITING
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

    void update_gamestage_first_commit();
    void update_gamestage_commiting();

    void update_available_moves();

private:

    Vec2i const CURSOR_COLLISION_BOX_WIDTH_HEIGHT;

    GameStage cur_stage{ GameStage::INIT_COMMIT };
    ColourField<COLOUR_FIELD_SIZE, N_BLOCKS_IN_ROW, Vec2i{ COLOUR_FIELD_AREA.left, COLOUR_FIELD_AREA.top }> blocks{ MAIN_COLOURS };

    Vec2i const& cursor_pos;

    std::size_t            cur_colour_index{ 0U };
    decltype(blocks.end()) cur_block{ blocks.end() };
    decltype(blocks.get_adject_blocks_with_not_equal_color(cur_block, MAIN_COLOURS[cur_colour_index])) adject_cur_blocks{};

    GameEngine::FrameTimer ft{ };

    PulsationEffect pulsator{ BACKGROUND_COLOUR, MAX_BLOCK_TRANSPERENCY };
    std::function<GameEngine::Colour(GameEngine::Colour)> pulsation{ [&pulsator = this->pulsator](GameEngine::Colour c) { return pulsator(c); } };
};