#pragma once

#include "../Game.hpp"
#include "../GraphicsDirect2D.hpp"
#include "../IWindow.hpp"
#include "../FrameTimer.hpp"

#include "Paddle.hpp"
#include "PlayField.hpp"
#include "Brick.hpp"


class Arkanoid final : public GameEngine::Game
{
private:

    enum class GameStage
    {
        START, IN_PROGRESS, GAMEOVER
    };

public:

    static constexpr int WINDOW_WIDTH{ 800 };
    static constexpr int WINDOW_HEIGHT{ 600 };
    static constexpr bool RESIZABLE{ false };

public:

    Arkanoid() = delete;
    Arkanoid(GameEngine::Interfaces::IWindow& window, GameEngine::Interfaces::IFramableGraphics2D& graphics);
    Arkanoid(Arkanoid const&) = delete;
    Arkanoid(Arkanoid&&) = delete;

    ~Arkanoid() = default;

    Arkanoid& operator=(Arkanoid const&) = delete;
    Arkanoid& operator=(Arkanoid&&) = delete;

    virtual void update() override;
    virtual void render() override;

private:

    void update_start_stage();
    void update_in_progress_stage(float dt);
    void update_gameover_stage();

private:
    
    static constexpr GameEngine::Geometry::Rectangle2D<int> PADDING{ 230, 30, 30, 30 };
    static constexpr GameEngine::Geometry::Vector2D<int> PADDLE_INIT_POS{ (WINDOW_WIDTH - PADDING.left - PADDING.right) / 2 + PADDING.left, WINDOW_HEIGHT - PADDING.bottom - 60 };

    static constexpr int PADDLE_INIT_HALF_WIDTH{ 40 };
    static constexpr float PADDLE_INIT_SPEED{ 200.f };

    static constexpr int N_ROWS_BRICKS{ 4 };
    static constexpr GameEngine::Colour ROW_COLOURS[N_ROWS_BRICKS]{ GameEngine::Colours::INDIGO, GameEngine::Colours::GOLD, GameEngine::Colours::AZURE, GameEngine::Colours::STEEL_BLUE };
    static constexpr int N_BRICKS_IN_ROW{ (WINDOW_WIDTH - PADDING.left - PADDING.right) / Brick::WIDTH };
    static constexpr int N_BRICKS_TOTAL { N_ROWS_BRICKS* N_BRICKS_IN_ROW };
    static constexpr GameEngine::Geometry::Vector2D<int> GRID_BRICKS_BEG{ PADDING.left + (WINDOW_WIDTH - PADDING.left - PADDING.right) % Brick::WIDTH / 2, PADDING.top + 40 };

    static constexpr GameEngine::Geometry::Vector2D<float> BALL_INIT_VELOCITY{ 0.f, 200.f };
    static constexpr GameEngine::Geometry::Vector2D<int>   BALL_INIT_POS{ PADDLE_INIT_POS.x, PADDLE_INIT_POS.y - 100 };

    GameStage cur_stage{ GameStage::START };

    PlayField field;
    Paddle pad;
    GameEngine::FrameTimer ft{ };
    std::vector<Brick> bricks;
    Ball ball;
};