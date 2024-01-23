#pragma once

#include "../Game.hpp"
#include "../GraphicsDirect2D.hpp"
#include "../IWindow.hpp"

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

    virtual void render() override;

private:
    
    static constexpr int PADDING_LEFT{ 80U };
    static constexpr int PADDING_RIGHT{ 20U };
    static constexpr int PADDING_TOP{ 20U };
    static constexpr int PADDING_BOTTOM{ 20U };


    PlayField field;
};