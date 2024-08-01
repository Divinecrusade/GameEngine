#pragma once

#include <Game.hpp>
#include <GraphicsDirect2D.hpp>
#include <IWindow.hpp>
#include <FrameTimer.hpp>
#include <Animation.hpp>

#include "Paddle.hpp"
#include "PlayField.hpp"
#include "Brick.hpp"
#include "Missile.hpp"
#include "Blow.hpp"
#include "LifeCounter.hpp"

#include <stack>


class Arkanoid final : public GameEngine::Game
{
private:

    enum class GameStage
    {
        START, IN_PROGRESS, RESET, GAMEOVER
    };

    static constexpr wchar_t const* const ASSETS_DIR{ L"data\\assets\\" };
    static constexpr wchar_t const* const ASSET_GAMESTART_IMG { L"gamestart.bmp" };
    static constexpr wchar_t const* const ASSET_GAMEOVER_IMG  { L"gameover.bmp" };
    static constexpr wchar_t const* const ASSET_LIFE_SPRITE   { L"heart.bmp" };
    static constexpr wchar_t const* const ASSET_MISSILE_SPRITE{ L"missile.bmp" };
    static constexpr wchar_t const* const ASSET_BLOW_ANIMATION{ L"blow.bmp" };

    using Vec2f = GameEngine::Geometry::Vector2D<float>;
    using Vec2i = GameEngine::Geometry::Vector2D<int>;
    using Rec2i = GameEngine::Geometry::Rectangle2D<int>;

public:

    static constexpr Rec2i WINDOW  { 0, 800, 600, 0 };
    static constexpr bool RESIZABLE{ false };

public:

    Arkanoid() = delete;
    Arkanoid(GameEngine::Interfaces::IWindow& window, GameEngine::Interfaces::IFramableGraphics2D& graphics);
    Arkanoid(Arkanoid const&) = delete;
    Arkanoid(Arkanoid&&) = delete;

    ~Arkanoid() = default;

    Arkanoid& operator=(Arkanoid const&) = delete;
    Arkanoid& operator=(Arkanoid&&) = delete;

    void update() override;
    void render() override;

private:

    void update_start_stage();
    void update_in_progress_stage(float dt);
    void update_reset_stage(float dt);
    void update_gameover_stage();

    void update_paddle(float dt);
    void update_ball(float dt);
    void update_bricks();
    void update_missiles(float dt);
    void update_blows(float dt);

    void render_full_scene();

    void cascade_blows(Blow const& new_blow);

    void decrease_lives();

private:
    
    static constexpr Rec2i PADDING{ 250, 30, 30, 30 };
    static constexpr Vec2i PADDLE_INIT_POS
    { 
        (WINDOW.get_width() - PADDING.left - PADDING.right) / 2 + PADDING.left,
        WINDOW.get_height() - PADDING.bottom - 60 
    };

    static constexpr int   PADDLE_INIT_HALF_WIDTH{  40   };
    static constexpr float PADDLE_INIT_SPEED     { 250.f };

    static constexpr int N_ROWS_BRICKS{ 5 };
    static constexpr GameEngine::Colour ROW_COLOURS[N_ROWS_BRICKS]
    { 
        GameEngine::Colours::INDIGO, 
        GameEngine::Colours::YELLOW,
        GameEngine::Colours::AZURE,
        GameEngine::Colours::STEEL_BLUE,
        GameEngine::Colours::CRIMSON 
    };
    static constexpr int N_BRICKS_IN_ROW{ (WINDOW.get_width() - PADDING.left - PADDING.right) / Brick::WIDTH };
    static constexpr int N_BRICKS_TOTAL { N_ROWS_BRICKS* N_BRICKS_IN_ROW };
    static constexpr Vec2i GRID_BRICKS_BEG
    { 
        PADDING.left + (WINDOW.get_width() - PADDING.left - PADDING.right) % Brick::WIDTH / 2,
        PADDING.top + 50
    };

    static constexpr Vec2f BALL_INIT_DIR{ 0.f, 1.f };
    static constexpr float BALL_INIT_SPEED{ 400.f };
    static constexpr Vec2i BALL_INIT_POS{ PADDLE_INIT_POS.x, PADDLE_INIT_POS.y - 100 };

    static constexpr float MISSILE_SPEED{ 300.f };

    static constexpr float BLOW_DURATION{ 3.f };

    static constexpr int N_LIVES{ 3 };

    static constexpr Rec2i LIVES_AREA{ WINDOW.left + 30, WINDOW.left + PADDING.left, WINDOW.bottom - PADDING.bottom, WINDOW.top + PADDING.top };
    static constexpr float RESET_STAGE_DURATION{ 2.f };

    GameStage cur_stage{ GameStage::START };

    PlayField field;
    Paddle    pad;
    GameEngine::FrameTimer ft{ };
    std::vector<Brick>     bricks;
    Ball      ball;

    GameEngine::Surface   gamestart_img;
    GameEngine::Surface   gameover_img;
    GameEngine::Surface   rocket;
    GameEngine::Surface   heart;

    GameEngine::AnimationFrames blow_effect;

    std::vector<Missile> missiles;
    std::vector<Blow>    blows;

    LifeCounter lives;
};