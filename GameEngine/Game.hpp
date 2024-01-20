#pragma once

#include "MainWindow/IWindow.hpp"
#include "IGraphics2D.hpp"


namespace GameEngine2D
{
    class Game
    {
    public:

        Game() = delete;
        Game(Interfaces::IWindow& window, Interfaces::IGraphics2D& graphics);
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

        Interfaces::IWindow& wnd;
        Interfaces::IGraphics2D& gfx;
    };
}