#include "Game.hpp"

Game::Game(IWindow& window)
:
wnd{ window },
gfx{ wnd.get_window_handler() }
{ }

void Game::start()
{
    while (!wnd.is_terminated())
    {
        process();
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
    gfx.draw_line(0, 0, 120, 340, 4, D2D1::ColorF::AliceBlue);
    gfx.draw_rectangle(100, 200, 300, 300, D2D1::ColorF::Green);
    gfx.fill_rectangle(120, 200, 240, 250, D2D1::ColorF::Cyan);
}