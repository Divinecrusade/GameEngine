#pragma once

#include "IWindow.hpp"
#include "IFramableGraphics2D.hpp"
#include "WinApiException.hpp"


namespace GameEngine
{
    class Game
    {
    public:

        Game() = delete;
        Game(Interfaces::IWindow& window, Interfaces::IFramableGraphics2D& graphics) noexcept;
        Game(Game const&) = delete;
        Game(Game&&)      = delete;

        Game& operator=(Game const&) = delete;
        Game& operator=(Game&&)      = delete;

        virtual ~Game() noexcept = default;

        void start();
        void stop();

    protected:

        virtual void process();
        virtual void update();
        virtual void render();    
        
        Interfaces::IWindow const& get_wnd() const noexcept;

    private:

        Interfaces::IWindow& wnd;

    protected:

        Interfaces::IFramableGraphics2D& gfx;
    };
}