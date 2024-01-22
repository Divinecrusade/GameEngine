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
        gfx.draw_line({0, 0}, {static_cast<float>(gfx.get_screen_width()), static_cast<float>(gfx.get_screen_height())}, 2U, Colours::MAGENTA);
        gfx.draw_line({static_cast<float>(gfx.get_screen_width()), 0},  {0, static_cast<float>(gfx.get_screen_height())}, 2U, Colours::RED);

        gfx.draw_rectangle(Geometry::Rectangle2D<float>::get_from_center({ gfx.get_screen_width() / 2.f, gfx.get_screen_height() / 2.f}, 40.f, 40.f), 3.f, Colours::BLUE);
        gfx.fill_rectangle(Geometry::Rectangle2D<float>::get_from_center({ gfx.get_screen_width() / 2.f, gfx.get_screen_height() / 2.f}, 40.f, 40.f), Colours::GREEN);
    }

    Interfaces::IWindow const& Game::get_wnd() const noexcept
    {
        return wnd;
    }
}