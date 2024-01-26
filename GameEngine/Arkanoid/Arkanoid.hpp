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
    
    static constexpr int PADDING_LEFT{ 230 };
    static constexpr int PADDING_RIGHT{ 30 };
    static constexpr int PADDING_TOP{ 30 };
    static constexpr int PADDING_BOTTOM{ 30 };

    static constexpr int PADDLE_INIT_X{ (WINDOW_WIDTH - PADDING_LEFT - PADDING_RIGHT) / 2 + PADDING_LEFT };
    static constexpr int PADDLE_INIT_Y{ WINDOW_HEIGHT - PADDING_BOTTOM - 40 };
    static constexpr int PADDLE_INIT_HALF_WIDTH{ 40 };
    static constexpr float PADDLE_INIT_SPEED{ 200.f };

    static constexpr int N_ROWS_BRICKS{ 4 };
    static constexpr GameEngine::Colour ROW_COLOURS[N_ROWS_BRICKS]{ GameEngine::Colours::INDIGO, GameEngine::Colours::GOLD, GameEngine::Colours::AZURE, GameEngine::Colours::STEEL_BLUE };
    static constexpr int N_BRICKS_IN_ROW{ (WINDOW_WIDTH - PADDING_LEFT - PADDING_RIGHT) / Brick::WIDTH };
    static constexpr int N_BRICKS_TOTAL{ N_ROWS_BRICKS* N_BRICKS_IN_ROW };
    static constexpr int GRID_BRICKS_BEG_X{ PADDING_LEFT + (WINDOW_WIDTH - PADDING_LEFT - PADDING_RIGHT) % Brick::WIDTH / 2};
    static constexpr int GRID_BRICKS_BEG_Y{ PADDING_TOP + 40 };

    static constexpr int BALL_INIT_POS_X{ PADDLE_INIT_X - 80 };
    static constexpr int BALL_INIT_POS_Y{ PADDLE_INIT_Y - 80 };
    static constexpr float BALL_INIT_VEL_X{ 125.f };
    static constexpr float BALL_INIT_VEL_Y{ 125.f };


    PlayField field;
    Paddle pad;
    GameEngine::FrameTimer ft{ };
    std::vector<Brick> bricks;
    Ball ball;
};