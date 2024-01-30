#include "Arkanoid.hpp"


Arkanoid::Arkanoid(GameEngine::Interfaces::IWindow& window, GameEngine::Interfaces::IFramableGraphics2D& graphics)
:
Game{ window, graphics },
field{ GameEngine::Geometry::Rectangle2D{ 0 + PADDING.left, WINDOW_WIDTH - PADDING.right, WINDOW_HEIGHT - PADDING.bottom, 0 + PADDING.top } },
pad{ PADDLE_INIT_POS, PADDLE_INIT_SPEED, PADDLE_INIT_HALF_WIDTH },
ball{ BALL_INIT_POS, BALL_INIT_VELOCITY }
{ 
    GameEngine::Geometry::Vector2D<int> const brick_size{ Brick::WIDTH, Brick::HEIGHT };
    bricks.reserve(N_BRICKS_TOTAL);
    for (int i{ 0 }; i != N_BRICKS_TOTAL; ++i)
    {
        bricks.emplace_back(GRID_BRICKS_BEG + brick_size * GameEngine::Geometry::Vector2D<int>{ i % N_BRICKS_IN_ROW, i / N_BRICKS_IN_ROW }, ROW_COLOURS[i / N_BRICKS_IN_ROW] );
    }
}

void Arkanoid::update()
{
    float const dt{ ft.mark() };
    bool collision_happended{ false };

    ball.update(dt);
    Paddle::Direction new_dir{ pad.get_direction() };
    switch (get_wnd().get_last_pressed_functional_key())
    {
        case GameEngine::WinKey::ARROW_LEFT: new_dir = Paddle::Direction::LEFT; break;
        case GameEngine::WinKey::ARROW_RIGHT: new_dir = Paddle::Direction::RIGHT; break;
        default: 
            
            if (!get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_LEFT) && !get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_RIGHT))
                new_dir = Paddle::Direction::STOP;

        break;
    }
    pad.set_direction(new_dir);
    pad.update(dt);

    if (!field.is_in_field(pad)) field.handle_collision(pad);
    if (!field.is_in_field(ball)) 
    {
        field.handle_collision(ball);
        collision_happended = true;
    }
    if (pad.is_collided_with(ball)) pad.handle_collision(ball);

    auto collided_brick{ bricks.end() };
    int distance{ };
    for (auto brick{ bricks.begin() }; brick != bricks.end(); ++brick)
    {
        if (brick->is_colided_with(ball))
        {
            if (collided_brick == bricks.end()) 
            {
                collided_brick = brick;
                distance = collided_brick->get_sqr_distance(ball);

                continue;
            }
            int const tmp_distance{ brick->get_sqr_distance(ball) };
            if (distance > tmp_distance)
            {
                collided_brick = brick;
                distance = tmp_distance;

                break;
            }
        }
    }
    
    if (collided_brick != bricks.end())
    {
        collided_brick->handle_collision(ball);
        bricks.erase(std::remove(bricks.begin(), bricks.end(), *collided_brick), bricks.end());
        collision_happended = true;
    }

    if (collision_happended && pad.is_cooldowned()) pad.reset_cooldown();
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
    ball.draw(gfx);
}
