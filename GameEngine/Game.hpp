#pragma once

#include "MainWindow/IWindow.hpp"
#include "IGraphics2D.hpp"


class Game
{
public:

    Game() = delete;
    Game(IWindow& window, IGraphics2D& graphics);
    Game(Game const&) = delete;
    Game(Game&&) = delete;

    Game& operator=(Game const&) = delete;
    Game& operator=(Game&&) = delete;

    virtual ~Game() = default;

    void start();

protected:

    virtual void process();
    virtual void update();
    virtual void render();

private:

    IWindow& wnd;
    IGraphics2D& gfx;
};