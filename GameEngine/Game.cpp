#include "Game.hpp"


namespace GameEngine
{
    Game::Game(Interfaces::IWindow& window, Interfaces::IFramableGraphics2D& graphics) noexcept
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
        if (!PostMessageW(wnd.get_window_handler(), WM_DESTROY, NULL, NULL)) throw WinApiException{ "Failed to destroy window via posting WM_DESTROY message" };
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