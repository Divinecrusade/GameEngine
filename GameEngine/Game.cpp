#include "Game.hpp"

Game::Game(IWindow& window)
:
wnd{ window }
{ }

void Game::start()
{
    while (!wnd.is_terminated())
    {
        process();
        update();
        render();
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
}
