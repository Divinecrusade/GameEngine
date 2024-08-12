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

    void Game::stop()
    {
        assert(!wnd.is_terminated());
        PostMessage(wnd.get_window_handler(), WM_DESTROY, NULL, NULL);
    }

    void Game::process()
    {
        wnd.process_messages_queue();
    }

    void Game::update()
    { }

    void Game::render()
    { }

    Interfaces::IWindow const& Game::get_wnd() const noexcept
    {
        return wnd;
    }
}