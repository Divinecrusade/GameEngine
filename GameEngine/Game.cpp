#include "Game.hpp"


namespace GameEngine2D
{
    Game::Game(Interfaces::IWindow& window, Interfaces::IGraphics2D& graphics)
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
    }

    void Game::render()
    {
        gfx.draw_line(0, 0, gfx.get_screen_width(), gfx.get_screen_height(), 2U, Colours::MAGENTA);
        gfx.draw_line(gfx.get_screen_width(), 0,  0, gfx.get_screen_height(), 2U, Colours::RED);

    }

    Interfaces::IWindow const& Game::get_wnd() const noexcept
    {
        return wnd;
    }
}