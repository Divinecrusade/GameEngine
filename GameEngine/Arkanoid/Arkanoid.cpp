#include "Arkanoid.hpp"


Arkanoid::Arkanoid(GameEngine::Interfaces::IWindow& window, GameEngine::Interfaces::IFramableGraphics2D& graphics)
:
Game{ window, graphics },
field{ GameEngine::Geometry::Rectangle2D{ 0 + PADDING.left, WINDOW.get_width() - PADDING.right, WINDOW.get_height() - PADDING.bottom, 0 + PADDING.top}},
pad{ PADDLE_INIT_POS, PADDLE_INIT_SPEED, PADDLE_INIT_HALF_WIDTH },
ball{ BALL_INIT_POS, BALL_INIT_DIR, BALL_INIT_SPEED },
gamestart_img{ std::filesystem::current_path() / (std::filesystem::path{std::wstring{ ASSETS_DIR } + std::wstring{ L"gamestart.bmp" }}) },
gameover_img{ std::filesystem::current_path() / (std::filesystem::path{std::wstring{ ASSETS_DIR } + std::wstring{ L"gameover.bmp" }}) },
rocket{ std::filesystem::current_path() / (std::filesystem::path{std::wstring{ ASSETS_DIR } + std::wstring{ L"missile.bmp" }}) },
blow_effect{ std::filesystem::current_path() / (std::filesystem::path{std::wstring{ ASSETS_DIR } + std::wstring{ L"blow.bmp" }}), 50U, 70U, 0.125f }
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

    switch (cur_stage)
    {
        case Arkanoid::GameStage::START: update_start_stage(); break;        
        case Arkanoid::GameStage::IN_PROGRESS: update_in_progress_stage(dt); break;
        case Arkanoid::GameStage::GAMEOVER: update_gameover_stage(); break;
    }
}

void Arkanoid::update_start_stage()
{
    assert(cur_stage == GameStage::START);

    switch (get_wnd().get_last_pressed_functional_key())
    {
        case GameEngine::WinKey::ENTER: cur_stage = GameStage::IN_PROGRESS; break;
        default: break;
    }
}

void Arkanoid::update_in_progress_stage(float dt)
{
    assert(cur_stage == GameStage::IN_PROGRESS);

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
        if (field.is_in_lose_zone(ball))
        {
            cur_stage = GameStage::GAMEOVER;
            return;
        }
        field.handle_collision(ball);
        collision_happended = true;
    }
    if (pad.is_collided_with(ball)) pad.handle_collision(ball);


    std::vector<std::vector<std::shared_ptr<Missile>>::iterator> destroyed_missiles{ };
    std::stack<std::shared_ptr<Blow>> blows_to_process{ };
    for (auto missile{ missiles.begin() }; missile != missiles.end(); ++missile)
    {
        if (std::find(destroyed_missiles.begin(), destroyed_missiles.end(), missile) != destroyed_missiles.end()) continue;
        
        missile->get()->update(dt);

        if (missile->get()->is_collided_with(ball) || missile->get()->is_collided_with(field) || missile->get()->is_collided_with(pad))
        {
            kaboom(missile, destroyed_missiles, blows_to_process);
        }
        if (cur_stage == GameStage::GAMEOVER) return;

        while (!blows_to_process.empty())
        {
            auto blow{ blows_to_process.top() };
            blows_to_process.pop();

            if (blow.get()->is_collided_with(ball))
            {
                blows.back().get()->throw_ball(ball);
                collision_happended = true;
            }
            for (auto missile{ missiles.begin() }; missile != missiles.end(); ++missile)
            {
                if (std::find(destroyed_missiles.begin(), destroyed_missiles.end(), missile) != destroyed_missiles.end()) continue;

                if (blow.get()->is_collided_with(**missile))
                {
                    kaboom(missile, destroyed_missiles, blows_to_process);
                }
                if (cur_stage == GameStage::GAMEOVER) return;
            }
        }
    }

    if (destroyed_missiles.size() > 0U)
    {
        for (auto& destroyed_missile : destroyed_missiles)
        {
            std::remove_if(missiles.begin(), missiles.end(), [&destroyed_missile](std::shared_ptr<Missile> const& val){ return val.get() == destroyed_missile->get(); });
        }
        auto tmp{ missiles.begin() };
        std::advance(tmp, missiles.size() - destroyed_missiles.size());
        missiles.erase(tmp, missiles.end());
    }

    std::vector<std::vector<std::shared_ptr<Blow>>::iterator> ended_blows{};
    for (auto blow{ blows.begin() }; blow != blows.end(); ++blow)
    {
        blow->get()->update(dt);
        if (blow->get()->is_ended())
        {
            ended_blows.push_back(blow);
        }
    }

    if (ended_blows.size() > 0U)
    {
        for (auto& ended_blow : ended_blows)
        {
            std::remove_if(blows.begin(), blows.end(), [&ended_blow](std::shared_ptr<Blow>const& val){ return val.get() == ended_blow->get(); });
        }
        auto tmp{ blows.begin() };
        std::advance(tmp, blows.size() - ended_blows.size());
        blows.erase(tmp, blows.end());
    }

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
        missiles.emplace_back(std::make_shared<Missile>(GameEngine::Geometry::Vector2D<int>{ ball.get_center().x, PADDING.top - Missile::COLLISION_HALF_HEIGHT * 2 }, MISSILE_SPEED, rocket, GameEngine::Colours::WHITE));
        bricks.erase(std::remove(bricks.begin(), bricks.end(), *collided_brick), bricks.end());
        if (bricks.size() == 0U)
        {
            cur_stage = GameStage::GAMEOVER;
            return;
        }

        collision_happended = true;
    }

    if (collision_happended) pad.reset_cooldown();
}

void Arkanoid::update_gameover_stage()
{
    assert(cur_stage == GameStage::GAMEOVER);
}

void Arkanoid::render_full_scene()
{
    field.draw(gfx);
    pad.draw(gfx);
    for (auto const& brick : bricks)
    {
        brick.draw(gfx);
    }
    for (auto& missile : missiles)
    {
        missile.get()->draw(gfx, field.get_collision_box());
    }
    for (auto& blow : blows)
    {
        blow.get()->draw(gfx, field.get_collision_box());
    }
    ball.draw(gfx);
}

void Arkanoid::kaboom(std::vector<std::shared_ptr<Missile>>::iterator const& missile, std::vector<std::vector<std::shared_ptr<Missile>>::iterator>& destroyed_missiles, std::stack<std::shared_ptr<Blow>>& blows_to_process)
{
    destroyed_missiles.push_back(missile);
    blows.emplace_back(std::make_shared<Blow>(missile->get()->get_pos(), blow_effect, GameEngine::Colours::WHITE));

    if (missile->get()->is_collided_with(pad) || blows.back().get()->is_collided_with(pad))
    {
        cur_stage = GameStage::GAMEOVER;

        return;
    }

    blows_to_process.push(blows.back());
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

            gfx.draw_sprite_excluding_color({ WINDOW.get_width() / 2 - static_cast<int>(gameover_img.get_width() / 2U), WINDOW.get_height() / 2 - static_cast<int>(gameover_img.get_height() / 2U)}, gameover_img, GameEngine::Colours::BLACK, WINDOW);

        break;
    }
}
