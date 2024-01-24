#pragma once

#include "../Game.hpp"
#include "../GraphicsDirect2D.hpp"
#include "../IWindow.hpp"
#include "../FrameTimer.hpp"

#include "Paddle.hpp"
#include "PlayField.hpp"


class Arkanoid final : public GameEngine2D::Game
{
public:

    static constexpr int WINDOW_WIDTH{ 800 };
    static constexpr int WINDOW_HEIGHT{ 600 };
    static constexpr bool RESIZABLE{ false };

public:

    Arkanoid() = delete;
    Arkanoid(GameEngine2D::Interfaces::IWindow& window, GameEngine2D::Interfaces::IGraphics2D& graphics);
    Arkanoid(Arkanoid const&) = delete;
    Arkanoid(Arkanoid&&) = delete;

    ~Arkanoid() = default;

    Arkanoid& operator=(Arkanoid const&) = delete;
    Arkanoid& operator=(Arkanoid&&) = delete;

    virtual void update() override;
    virtual void render() override;

private:
    
    static constexpr int PADDING_LEFT{ 80 };
    static constexpr int PADDING_RIGHT{ 20 };
    static constexpr int PADDING_TOP{ 20 };
    static constexpr int PADDING_BOTTOM{ 20 };

    static constexpr int PADDLE_INIT_X{ (WINDOW_WIDTH - PADDING_LEFT - PADDING_RIGHT) / 2 + PADDING_LEFT };
    static constexpr int PADDLE_INIT_Y{ WINDOW_HEIGHT - PADDING_BOTTOM - 40 };
    static constexpr int PADDLE_INIT_HALF_WIDTH{ 40 };
    static constexpr float PADDLE_INIT_SPEED{ 125.f };


    PlayField field;
    Paddle pad;
    GameEngine2D::FrameTimer ft{ };
};