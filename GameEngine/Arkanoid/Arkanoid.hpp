#pragma once

#include <Game.hpp>
#include <GraphicsDirect2D.hpp>
#include <IWindow.hpp>
#include <FrameTimer.hpp>
#include <Animation.hpp>
#include <Mouse.hpp>

#include "Paddle.hpp"
#include "PlayField.hpp"
#include "Brick.hpp"
#include "Missile.hpp"
#include "Blow.hpp"
#include "LifeCounter.hpp"
#include "Score.hpp"


class Arkanoid final : public GameEngine::Game
{
private:

    enum class GameStage
    {
        START, IN_PROGRESS, RESET, GAMEOVER
    };

    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr Rec2i WINDOW   { 0, 800, 600, 0 };
    static constexpr bool  RESIZABLE{ false };

public:

    Arkanoid() = delete;
    Arkanoid(GameEngine::Interfaces::IWindow& window, GameEngine::Interfaces::IFramableGraphics2D& graphics);
    Arkanoid(Arkanoid const&) = delete;
    Arkanoid(Arkanoid&&)      = delete;

    ~Arkanoid() noexcept = default;

    Arkanoid& operator=(Arkanoid const&) = delete;
    Arkanoid& operator=(Arkanoid&&)      = delete;

    void update() noexcept override;
    void render() override;

private:

    void update_start_stage() noexcept;
    void update_in_progress_stage(float dt) noexcept;
    void update_reset_stage(float dt) noexcept;
    void update_gameover_stage() noexcept;

    void update_paddle(float dt) noexcept;
    void update_ball(float dt) noexcept;
    void update_bricks() noexcept;
    void update_missiles(float dt) noexcept;
    void update_blows(float dt) noexcept;

    void render_full_scene();

    void cascade_blows(Blow const& new_blow) noexcept;

    void decrease_lives() noexcept;

    void spawn_bricks() noexcept;

private:
    
    static constexpr std::wstring_view ASSETS_DIR{ L"data\\assets\\" };
    static constexpr std::wstring_view ASSET_GAMESTART_IMG{ L"gamestart.bmp" };
    static constexpr std::wstring_view ASSET_LIFE_SPRITE{ L"heart.bmp" };
    static constexpr std::wstring_view ASSET_MISSILE_SPRITE{ L"missile.bmp" };
    static constexpr std::wstring_view ASSET_BLOW_ANIMATION{ L"blow.bmp" };

    static constexpr Rec2i PADDING{ 30, 30, 30, 30 };
    static constexpr Rec2i PLAYFIELD_AREA{ PADDING.left + 220, PADDING.right, PADDING.bottom, PADDING.top };
    static constexpr Vec2i PADDLE_INIT_POS
    { 
        (WINDOW.get_width() - PLAYFIELD_AREA.left - PLAYFIELD_AREA.right) / 2 + PLAYFIELD_AREA.left,
        WINDOW.get_height() - PLAYFIELD_AREA.bottom - 60 
    };

    static constexpr int   PADDLE_INIT_HALF_WIDTH{  40   };
    static constexpr float PADDLE_INIT_SPEED     { 250.f };

    static constexpr int N_ROWS_BRICKS{ 5 };
    static constexpr std::array<GameEngine::Colour, N_ROWS_BRICKS> ROW_COLOURS
    { 
        GameEngine::Colours::INDIGO, 
        GameEngine::Colours::YELLOW,
        GameEngine::Colours::AZURE,
        GameEngine::Colours::STEEL_BLUE,
        GameEngine::Colours::CRIMSON 
    };
    static constexpr int N_BRICKS_IN_ROW{ (WINDOW.get_width() - PLAYFIELD_AREA.left - PLAYFIELD_AREA.right) / Brick::WIDTH };
    static constexpr int N_BRICKS_TOTAL { N_ROWS_BRICKS* N_BRICKS_IN_ROW };
    static constexpr Vec2i GRID_BRICKS_BEG
    { 
        PLAYFIELD_AREA.left + (WINDOW.get_width() - PLAYFIELD_AREA.left - PLAYFIELD_AREA.right) % Brick::WIDTH / 2,
        PLAYFIELD_AREA.top + 50
    };

    static constexpr Vec2f BALL_INIT_DIR{ 0.f, 1.f };
    static constexpr float BALL_INIT_SPEED{ 400.f };
    static constexpr Vec2i BALL_INIT_POS{ PADDLE_INIT_POS.x, PADDLE_INIT_POS.y - 100 };

    static constexpr float MISSILE_SPEED{ 300.f };

    static constexpr float BLOW_DURATION{ 2.f };

    static constexpr int N_LIVES{ 3 };

    static constexpr Rec2i LIVES_AREA{ WINDOW.left + 30, WINDOW.left + PLAYFIELD_AREA.left, PLAYFIELD_AREA.top + 100, WINDOW.top + PLAYFIELD_AREA.top };
    static constexpr float RESET_STAGE_DURATION{ 2.f };

    static constexpr Rec2i POINTS_AREA{ LIVES_AREA.left, LIVES_AREA.right, WINDOW.bottom - PLAYFIELD_AREA.bottom, LIVES_AREA.bottom };
    static constexpr Vec2i POINTS_LEFT_TOP_POS{ POINTS_AREA.left, POINTS_AREA.top };

    static constexpr GameEngine::Colour C1{ 75, 166, 226 };
    static constexpr GameEngine::Colour C2{ 74, 83, 128 };

    static constexpr int   BUTTON_AREA_OFFSET{ 50 };
    static constexpr Rec2i YES_BUTTON_AREA{ 30 + BUTTON_AREA_OFFSET, WINDOW.right / 2 - 30 - BUTTON_AREA_OFFSET, WINDOW.bottom - 30 - BUTTON_AREA_OFFSET, WINDOW.bottom / 2 + 30 + BUTTON_AREA_OFFSET };
    static constexpr Rec2i NO_BUTTON_AREA { WINDOW.right / 2 + 30 + BUTTON_AREA_OFFSET, WINDOW.right - 30 - BUTTON_AREA_OFFSET, WINDOW.bottom - 30 - BUTTON_AREA_OFFSET, WINDOW.bottom / 2 + 30 + BUTTON_AREA_OFFSET };
    
    static constexpr int   BUTTON_BORDER_WIDTH{ 5 };

    bool is_yes_btn_hovered{ false };
    bool is_no_btn_hovered { false };

    GameStage cur_stage{ GameStage::START };

    PlayField field;
    Paddle    pad;
    GameEngine::FrameTimer ft{ };
    std::vector<Brick>     bricks;
    Ball      ball;

    GameEngine::Surface   gamestart_img;
    GameEngine::Surface   rocket;
    GameEngine::Surface   heart;

    GameEngine::AnimationFrames blow_effect;

    std::vector<Missile> missiles;
    std::vector<Blow>    blows;

    LifeCounter lives;
    Score       points_counter;
};