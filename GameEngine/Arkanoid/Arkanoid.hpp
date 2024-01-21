#pragma once

#include "../Game.hpp"
#include "../GraphicsDirect2D.hpp"
#include "../MainWindow/IWindow.hpp"


class Arkanoid final : public GameEngine2D::Game
{
public:

    Arkanoid() = delete;
    Arkanoid(GameEngine2D::Interfaces::IWindow& window, GameEngine2D::Interfaces::IGraphics2D& graphics);
    Arkanoid(Arkanoid const&) = delete;
    Arkanoid(Arkanoid&&) = delete;

    ~Arkanoid() = default;

    Arkanoid& operator=(Arkanoid const&) = delete;
    Arkanoid& operator=(Arkanoid&&) = delete;

private:
    
};