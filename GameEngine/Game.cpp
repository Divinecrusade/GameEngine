#include "Game.hpp"

Game::Game(IWindow& window)
:
wnd{ window },
graphics{ wnd.get_window_handler() }
{ }

void Game::start()
{
    while (!wnd.is_terminated())
    {
        process();
        update();
        graphics.render_target->BeginDraw();
        graphics.render_target->SetTransform(D2D1::Matrix3x2F::Identity());
        graphics.render_target->Clear(D2D1::ColorF(D2D1::ColorF::White));
        render();
        graphics.render_target->EndDraw();
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
    auto const rtSize{ graphics.render_target->GetSize() };
    int width = static_cast<int>(rtSize.width);
    int height = static_cast<int>(rtSize.height);

    for (int x = 0; x < width; x += 10)
    {
        graphics.render_target->DrawLine(
            D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
            D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
            graphics.light_slate_gray,
            0.5f
        );
    }

    for (int y = 0; y < height; y += 10)
    {
        graphics.render_target->DrawLine(
            D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
            D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
            graphics.light_slate_gray,
            0.5f
        );
    }

    D2D1_RECT_F rectangle = D2D1::RectF(
        rtSize.width / 2 - 50.0f,
        rtSize.height / 2 - 50.0f,
        rtSize.width / 2 + 50.0f,
        rtSize.height / 2 + 50.0f
    );

    graphics.render_target->FillRectangle(&rectangle, graphics.light_slate_gray);
}