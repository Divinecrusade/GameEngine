#include "Arkanoid.hpp"


Arkanoid::Arkanoid(GameEngine2D::Interfaces::IWindow& window, GameEngine2D::Interfaces::IFramableGraphics2D& graphics)
:
Game{ window, graphics },
field{ GameEngine2D::Geometry::Rectangle2D{ 0 + PADDING_LEFT, WINDOW_WIDTH - PADDING_RIGHT, WINDOW_HEIGHT - PADDING_BOTTOM, 0 + PADDING_TOP } },
pad{ GameEngine2D::Geometry::Vector2D<int>{ PADDLE_INIT_X, PADDLE_INIT_Y }, PADDLE_INIT_SPEED, PADDLE_INIT_HALF_WIDTH }
{ 
    GameEngine2D::Geometry::Vector2D<int> const brick_size{ Brick::WIDTH, Brick::HEIGHT };
    GameEngine2D::Geometry::Vector2D<int> const grid_beg{ GRID_BRICKS_BEG_X, GRID_BRICKS_BEG_Y };
    bricks.reserve(N_BRICKS_TOTAL);
    for (int i{ 0 }; i != N_BRICKS_TOTAL; ++i)
    {
        bricks.emplace_back(grid_beg + brick_size * GameEngine2D::Geometry::Vector2D<int>{ i % N_BRICKS_IN_ROW, i / N_BRICKS_IN_ROW }, ROW_COLOURS[i / N_BRICKS_IN_ROW] );
    }
}

void Arkanoid::update()
{
    float const dt{ ft.mark() };

    Paddle::Direction new_dir{ pad.get_direction() };
    switch (get_wnd().get_last_pressed_functional_key())
    {
        case GameEngine2D::WinKey::ARROW_LEFT: new_dir = Paddle::Direction::LEFT; break;
        case GameEngine2D::WinKey::ARROW_RIGHT: new_dir = Paddle::Direction::RIGHT; break;
        default: 
            
            if (!get_wnd().is_fun_key_pressed(GameEngine2D::WinKey::ARROW_LEFT) && !get_wnd().is_fun_key_pressed(GameEngine2D::WinKey::ARROW_RIGHT))
                new_dir = Paddle::Direction::STOP;

        break;
    }
    pad.set_direction(new_dir);
    pad.update(dt);
    if (!field.is_in_field(pad)) field.handle_collision(pad);
}

void Arkanoid::render()
{
    Game::render();
    field.draw(gfx);
    pad.draw(gfx);
    for (auto const& brick : bricks)
    {
        brick.draw(gfx);
    }
}
