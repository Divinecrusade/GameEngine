#include "Game.hpp"

namespace GameEngine
{
    Game::Game(Interfaces::IWindow& window, Interfaces::IFramableGraphics2D& graphics)
    :
    wnd{ window },
    gfx{ graphics }
    { }

    void Game::start()
    {
        for (; !wnd.is_terminated(); process())
        {
            update();
            gfx.begin_frame();
            render();
            gfx.end_frame();
        }
    }

    void Game::process()
    {
        wnd.process_messages_queue();
    }

    void Game::update()
    {
        auto dt{ ft.mark() };

        test.update(dt);
    }

    void Game::render()
    {
        test.draw(gfx, {120, 200}, Colours::MAGENTA);
    }

    Interfaces::IWindow const& Game::get_wnd() const noexcept
    {
        return wnd;
    }
}