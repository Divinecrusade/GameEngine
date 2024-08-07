#include "Arkanoid.hpp"


Arkanoid::Arkanoid(GameEngine::Interfaces::IWindow& window, GameEngine::Interfaces::IFramableGraphics2D& graphics)
:
Game { window, graphics },
field{ Rec2i{ 0 + PADDING.left, WINDOW.get_width() - PADDING.right, WINDOW.get_height() - PADDING.bottom, 0 + PADDING.top } },
pad  { PADDLE_INIT_POS, PADDLE_INIT_SPEED, PADDLE_INIT_HALF_WIDTH },
ball { BALL_INIT_POS, BALL_INIT_DIR, BALL_INIT_SPEED },
gamestart_img{ std::filesystem::current_path() / (std::filesystem::path{ std::wstring{ ASSETS_DIR } + std::wstring{ ASSET_GAMESTART_IMG  }}) },
gameover_img { std::filesystem::current_path() / (std::filesystem::path{ std::wstring{ ASSETS_DIR } + std::wstring{ ASSET_GAMEOVER_IMG   }}) },
rocket       { std::filesystem::current_path() / (std::filesystem::path{ std::wstring{ ASSETS_DIR } + std::wstring{ ASSET_MISSILE_SPRITE }}) },
blow_effect  { std::filesystem::current_path() / (std::filesystem::path{ std::wstring{ ASSETS_DIR } + std::wstring{ ASSET_BLOW_ANIMATION }}), 50U, 70U }
{ 
    constexpr Vec2i brick_size{ Brick::WIDTH, Brick::HEIGHT };
    bricks.reserve(N_BRICKS_TOTAL);
    for (int i{ 0 }; i != N_BRICKS_TOTAL; ++i)
    {
        bricks.emplace_back(GRID_BRICKS_BEG + brick_size * Vec2i{ i % N_BRICKS_IN_ROW, i / N_BRICKS_IN_ROW }, ROW_COLOURS[i / N_BRICKS_IN_ROW] );
    }
}

void Arkanoid::update()
{
    switch (float const dt{ ft.mark() }; cur_stage)
    {
        case Arkanoid::GameStage::START:       update_start_stage();         break;        
        case Arkanoid::GameStage::IN_PROGRESS: update_in_progress_stage(dt); break;
        case Arkanoid::GameStage::GAMEOVER:    update_gameover_stage();      break;
    }
}

void Arkanoid::update_start_stage()
{
    assert(cur_stage == GameStage::START);

    if (auto const functional_key{ get_wnd().get_last_pressed_functional_key() }; functional_key.has_value())
        switch (functional_key.value())
        {
            case GameEngine::WinKey::ENTER: cur_stage = GameStage::IN_PROGRESS; break;
            default: break;
        }
}

void Arkanoid::update_in_progress_stage(float dt)
{
    assert(cur_stage == GameStage::IN_PROGRESS);

    update_paddle(dt);
    update_ball(dt);
    update_bricks();
    update_missiles(dt);
    update_blows(dt);
}

void Arkanoid::update_gameover_stage()
{
    assert(cur_stage == GameStage::GAMEOVER);
}

void Arkanoid::update_paddle(float dt)
{
    Paddle::Direction new_dir{ pad.get_direction() };

    if (auto const functional_key{ get_wnd().get_last_pressed_functional_key() }; functional_key.has_value())
        switch (functional_key.value())
        {
            case GameEngine::WinKey::ARROW_LEFT:  new_dir = Paddle::Direction::LEFT;  break;
            case GameEngine::WinKey::ARROW_RIGHT: new_dir = Paddle::Direction::RIGHT; break;
            default:

                if (!get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_LEFT) && !get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_RIGHT))
                    new_dir = Paddle::Direction::STOP;

            break;
        }
    else new_dir = Paddle::Direction::STOP;
    pad.set_direction(new_dir);
    
    pad.update(dt);
    if (!field.is_in_field(pad)) field.handle_collision(pad);
}

void Arkanoid::update_ball(float dt)
{
    ball.update(dt);
    if (!field.is_in_field(ball))
    {
        if (field.is_in_lose_zone(ball))
        {
            cur_stage = GameStage::GAMEOVER;

            return;
        }
        field.handle_collision(ball);
        pad.reset_cooldown();
    }
    if (pad.is_collided_with(ball)) pad.deflect(ball);
}

void Arkanoid::update_bricks()
{
    auto collided_brick{ bricks.end() };
    int distance{ Ball::RADIUS * Ball::RADIUS / 2 };
    for (auto brick{ bricks.begin() }; brick != bricks.end(); ++brick)
    {
        if (brick->is_collided_with(ball))
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
        collided_brick->deflect(ball);

        missiles.emplace_back(Vec2i{ ball.get_collision_box().get_center().x, PADDING.top - Missile::COLLISION_HALF_HEIGHT * 2 }, MISSILE_SPEED, rocket, GameEngine::Colours::MAGENTA);
        if (bricks.erase(collided_brick); bricks.empty())
        {
            cur_stage = GameStage::GAMEOVER;

            return;
        }

        pad.reset_cooldown();
    }
}

void Arkanoid::update_missiles(float dt)
{
    for (auto missile{ missiles.begin() }; missile != missiles.end(); ++missile)
    {
        if (missile->is_destroyed()) continue;
        missile->update(dt);

        if (missile->is_collided_with(ball) || 
            missile->is_collided_with(pad)  || 
            field.get_collision_box().bottom < missile->get_collision_box().bottom)
        {
            missile->destroy();

            cascade_blows(std::cref(blows.emplace_back(missile->get_pos(), GameEngine::Animation{ blow_effect, BLOW_DURATION }, GameEngine::Colours::MAGENTA)));
        }
    }
    std::erase_if(missiles, [](Missile const& missile)
        {
            return missile.is_destroyed();
        });
}

void Arkanoid::update_blows(float dt)
{
    for (auto& blow : blows)
    {
        blow.update(dt);
    }
    std::erase_if(blows, [](Blow const& blow)
        {
            return blow.is_ended();
        });
}

void Arkanoid::render_full_scene()
{
    field.draw(gfx);
    pad.draw(gfx);
    for (auto const& brick : bricks)
    {
        brick.draw(gfx);
    }
    for (auto const& missile : missiles)
    {
        missile.draw(gfx, field.get_collision_box());
    }
    for (auto const& blow : blows)
    {
        blow.draw(gfx, WINDOW);
    }
    ball.draw(gfx);
}

void Arkanoid::cascade_blows(Blow const& new_blow)
{
    if (new_blow.is_collided_with(ball)) new_blow.throw_ball(ball);

    for (auto missile{ missiles.begin() }; missile != missiles.end(); ++missile)
    {
        if (missile->is_destroyed()) continue;

        if (new_blow.is_collided_with(*missile))
        {
            missile->destroy();
            cascade_blows(std::cref(blows.emplace_back(missile->get_pos(), GameEngine::Animation{ blow_effect, BLOW_DURATION }, GameEngine::Colours::MAGENTA)));
        }
    }
}

void Arkanoid::render()
{
    Game::render();
    switch (cur_stage)
    {
        case GameStage::START: 

            gfx.draw_sprite({ 0, WINDOW.get_height() / 2 - static_cast<int>(gamestart_img.get_height() / 2U) }, gamestart_img, WINDOW);
        
        break;

        case GameStage::IN_PROGRESS:
        
            render_full_scene();

        break;

        case GameStage::GAMEOVER:

            render_full_scene();

            gfx.draw_sprite_excluding_color
            (
                { 
                    WINDOW.get_width()  / 2 - static_cast<int>(gameover_img.get_width()  / 2U),
                    WINDOW.get_height() / 2 - static_cast<int>(gameover_img.get_height() / 2U)
                },
                gameover_img,
                GameEngine::Colours::BLACK,
                WINDOW
            );

        break;
    }
}
