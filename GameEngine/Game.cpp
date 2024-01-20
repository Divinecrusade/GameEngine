#include "Game.hpp"

Game::Game(IWindow& window, IGraphics2D& graphics)
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
    auto const w{ gfx.get_screen_width() };
    auto const h{ gfx.get_screen_height() };

    gfx.draw_rectangle(0, 0, 200, 300, 255, 0, 0); // red
    gfx.draw_rectangle(w - 200, h - 200, w, h, 255U, 0U, 0U); // red
    gfx.draw_line(0, 0, w, h, 3U, 0U, 255U, 0U); // green
    gfx.fill_rectangle(0, 0, 200, 300, 0U, 0U, 255U); // blue
}