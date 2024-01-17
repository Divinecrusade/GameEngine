#pragma once

#include "MainWindow/IWindow.hpp"


class Game
{
public:

    Game() = delete;
    Game(IWindow& window);
    Game(Game const&) = delete;
    Game(Game&&) = delete;

    Game& operator=(Game const&) = delete;
    Game& operator=(Game&&) = delete;

    virtual ~Game() = default;

    void Go();

protected:

    virtual void Process();
    virtual void Update();
    virtual void Render();

private:

    IWindow& wnd;
};