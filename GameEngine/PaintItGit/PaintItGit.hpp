#pragma once

#include <Game.hpp>
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>
#include <Rectangle2D.hpp>
#include <Mouse.hpp>
#include <FrameTimer.hpp>

#include <algorithm>

#include "ColourField.hpp"
#include "ColourGit.hpp"


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
    
    static constexpr int   HORIZONTAL_PADDING_BETWEEN_FIELD_AND_GIT{ 50 };

    static constexpr int   GIT_COLOUR_WIDTH { WINDOW.get_width() - PADDING.left - PADDING.right - HORIZONTAL_PADDING_BETWEEN_FIELD_AND_GIT - COLOUR_FIELD_SIZE };
    static constexpr int   GIT_COLOUR_HEIGHT{ COLOUR_FIELD_SIZE };

    static constexpr Rec2i GIT_COLOUR_AREA  { Vec2i{ COLOUR_FIELD_AREA.right + HORIZONTAL_PADDING_BETWEEN_FIELD_AND_GIT, COLOUR_FIELD_AREA.top }, GIT_COLOUR_WIDTH, GIT_COLOUR_HEIGHT };

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

    static constexpr float MAX_INPUT_DELAY{ 0.2f };

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

    PaintItGit()                  = delete;
    PaintItGit(PaintItGit const&) = delete;
    PaintItGit(PaintItGit&&)      = delete;

    PaintItGit& operator=(PaintItGit const&) = delete;
    PaintItGit& operator=(PaintItGit&&)      = delete;

    virtual ~PaintItGit() noexcept = default;

    using Game::start;

private:

    virtual void update();
    virtual void render();

    void update_gamestage_first_commit();
    void update_gamestage_commiting();

    void update_available_moves();

private:

    Vec2i const  CURSOR_COLLISION_BOX_WIDTH_HEIGHT;
    Vec2i const& cursor_pos;

    ColourField<COLOUR_FIELD_SIZE, N_BLOCKS_IN_ROW, Vec2i{ COLOUR_FIELD_AREA.left, COLOUR_FIELD_AREA.top }> blocks;
    ColourGit<GIT_COLOUR_AREA, BACKGROUND_COLOUR> git;

    GameStage cur_stage{ GameStage::INIT_COMMIT };

    std::size_t            cur_colour_index{ 0U };
    GameEngine::Colour     prev_colour{ };

    decltype(blocks)::iterator cur_block{ blocks.end() };
    std::array<decltype(blocks)::iterator, decltype(blocks)::MAX_N_ADJECT_BLOCKS> adject_cur_blocks{ };
    std::size_t n_available_adject_cur_blocks{ 0U };

    GameEngine::FrameTimer ft{ };
    PulsationEffect pulsator{ BACKGROUND_COLOUR, MAX_BLOCK_TRANSPERENCY };
    float cur_input_delay{ -MAX_INPUT_DELAY };
};