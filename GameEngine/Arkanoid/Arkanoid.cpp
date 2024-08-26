#include "Arkanoid.hpp"


Arkanoid::Arkanoid(GameEngine::Interfaces::IWindow& window, GameEngine::Interfaces::IFramableGraphics2D& graphics)
:
Game { window, graphics },
field{ Rec2i{ 0 + PLAYFIELD_AREA.left, WINDOW.get_width() - PLAYFIELD_AREA.right, WINDOW.get_height() - PLAYFIELD_AREA.bottom, 0 + PLAYFIELD_AREA.top } },
pad  { PADDLE_INIT_POS, PADDLE_INIT_SPEED, PADDLE_INIT_HALF_WIDTH },
ball { BALL_INIT_POS, BALL_INIT_DIR, BALL_INIT_SPEED },
gamestart_img { std::filesystem::current_path() / (std::filesystem::path{ std::wstring{ ASSETS_DIR } + std::wstring{ ASSET_GAMESTART_IMG  }}) },
rocket        { std::filesystem::current_path() / (std::filesystem::path{ std::wstring{ ASSETS_DIR } + std::wstring{ ASSET_MISSILE_SPRITE }}) },
heart         { std::filesystem::current_path() / (std::filesystem::path{ std::wstring{ ASSETS_DIR } + std::wstring{ ASSET_LIFE_SPRITE   }}) },
blow_effect   { std::filesystem::current_path() / (std::filesystem::path{ std::wstring{ ASSETS_DIR } + std::wstring{ ASSET_BLOW_ANIMATION }}), 50U, 70U },
lives         { 0, N_LIVES, { PLAYFIELD_AREA.right, PLAYFIELD_AREA.top }, heart, GameEngine::Colours::WHITE },
points_counter{ POINTS_LEFT_TOP_POS, C1 }
{ 
    bricks.reserve(N_BRICKS_TOTAL);
    spawn_bricks();
}

void Arkanoid::update() noexcept
{
    switch (float const dt{ ft.mark() }; cur_stage)
    {
        case Arkanoid::GameStage::START:       update_start_stage();         break;        
        case Arkanoid::GameStage::IN_PROGRESS: update_in_progress_stage(dt); break;
        case Arkanoid::GameStage::RESET:       update_reset_stage(dt);       break;
        case Arkanoid::GameStage::GAMEOVER:    update_gameover_stage();      break;
    }
}

void Arkanoid::update_start_stage() noexcept
{
    assert(cur_stage == GameStage::START);

    if (auto const functional_key{ get_wnd().get_last_pressed_functional_key() }; functional_key.has_value())
        switch (functional_key.value())
        {
            case GameEngine::WinKey::ENTER: cur_stage = GameStage::IN_PROGRESS; break;
            default: break;
        }
}

void Arkanoid::update_in_progress_stage(float dt) noexcept
{
    assert(cur_stage == GameStage::IN_PROGRESS);

    update_paddle(dt);
    update_ball(dt);
    update_bricks();
    update_missiles(dt);
    update_blows(dt);
}

void Arkanoid::update_reset_stage(float dt) noexcept
{
    assert(cur_stage == GameStage::RESET);

    static float left_duration{ RESET_STAGE_DURATION };

    if (GameEngine::Geometry::Auxiliry::is_equal_with_precision(left_duration, RESET_STAGE_DURATION))
    {
        missiles.clear();
        blows.clear();
        pad.move_to(PADDLE_INIT_POS);
        ball.reset();
        points_counter.ball_missed();
    }

    left_duration -= dt;
    if (left_duration <= 0.f)
    {
        left_duration = RESET_STAGE_DURATION;
        cur_stage = GameStage::IN_PROGRESS;
    }
}

void Arkanoid::update_gameover_stage() noexcept
{
    assert(cur_stage == GameStage::GAMEOVER);

    if (auto const mouse_pos{ GameEngine::Mouse::get_cursor_pos(get_wnd()) }; YES_BUTTON_AREA.contains(mouse_pos))
    {
        is_yes_btn_hovered = true;
        is_no_btn_hovered = false;
    }
    else if (NO_BUTTON_AREA.contains(mouse_pos))
    {
        is_yes_btn_hovered = false;
        is_no_btn_hovered = true;
    }
    else is_yes_btn_hovered = is_no_btn_hovered = false;

    if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::MOUSE_LEFT_BUTTON))
    {
        if (is_yes_btn_hovered)
        {
            ball.reset();
            pad.move_to(PADDLE_INIT_POS);
            missiles.clear();
            blows.clear();
            lives.reset();
            points_counter.reset();
            bricks.clear();
            spawn_bricks();
            cur_stage = GameStage::START;
        }
        else if (is_no_btn_hovered) stop();
    }
}

void Arkanoid::update_paddle(float dt) noexcept
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

void Arkanoid::update_ball(float dt) noexcept
{
    ball.update(dt);
    if (!field.is_in_field(ball))
    {
        if (field.is_in_lose_zone(ball))
        {
            decrease_lives();

            return;
        }
        field.handle_collision(ball);
        pad.reset_cooldown();
    }
    if (pad.is_collided_with(ball) && !pad.is_cooldowned())
    {
        pad.deflect(ball);
        points_counter.ball_reflected_by_paddle();
    }
}

void Arkanoid::update_bricks() noexcept
{
    if (auto const collided_brick{ std::ranges::find_if(bricks, [&ball = this->ball](auto const& brick) { return brick.is_collided_with(ball); })}; collided_brick != bricks.end())
    {
        collided_brick->deflect(ball);

        missiles.emplace_back(Vec2i{ ball.get_collision_box().get_center().x, PLAYFIELD_AREA.top }, MISSILE_SPEED, rocket, GameEngine::Colours::MAGENTA);
        if (bricks.erase(collided_brick); bricks.empty())
        {
            cur_stage = GameStage::GAMEOVER;

            return;
        }

        pad.reset_cooldown();
        points_counter.brick_destroyed(lives.get_cur_n_lives());
    }
}

void Arkanoid::update_missiles(float dt) noexcept
{
    for (auto missile{ missiles.begin() }; missile != missiles.end(); ++missile)
    {
        if (missile->is_destroyed()) continue;
        missile->update(dt);

        if (missile->is_collided_with(pad))
        {
            decrease_lives();

            return;
        }
        if (missile->is_collided_with(ball) || field.get_collision_box().bottom < missile->get_collision_box().bottom)
        {
            missile->destroy();
            pad.reset_cooldown();

            cascade_blows(blows.emplace_back(missile->get_pos(), GameEngine::Animation{ blow_effect, BLOW_DURATION }, GameEngine::Colours::MAGENTA));
        }
    }
    std::erase_if(missiles, [](Missile const& missile)
        {
            return missile.is_destroyed();
        });
}

void Arkanoid::update_blows(float dt) noexcept
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
    lives.draw(gfx, LIVES_AREA);
    points_counter.draw(gfx, POINTS_AREA);
}

void Arkanoid::cascade_blows(Blow const& new_blow) noexcept
{
    if (new_blow.is_collided_with(ball)) new_blow.throw_ball(ball);

    for (auto missile{ missiles.begin() }; missile != missiles.end(); ++missile)
    {
        if (missile->is_destroyed()) continue;

        if (new_blow.is_collided_with(*missile))
        {
            missile->destroy();
            cascade_blows(blows.emplace_back(missile->get_pos(), GameEngine::Animation{ blow_effect, BLOW_DURATION }, GameEngine::Colours::MAGENTA));
        }
    }
}

void Arkanoid::decrease_lives() noexcept
{
    if (lives.is_ended()) cur_stage = GameStage::GAMEOVER;
    else
    {
        lives.decrease();
        pad.reset_cooldown();
        cur_stage = GameStage::RESET;
    }
}

void Arkanoid::spawn_bricks() noexcept
{
    constexpr Vec2i brick_size{ Brick::WIDTH, Brick::HEIGHT };
    for (int i{ 0 }; i != N_BRICKS_TOTAL; ++i)
    {
        bricks.emplace_back(GRID_BRICKS_BEG + brick_size * Vec2i{ i % N_BRICKS_IN_ROW, i / N_BRICKS_IN_ROW }, ROW_COLOURS[static_cast<std::array<GameEngine::Colour, 5>::size_type>(i / N_BRICKS_IN_ROW)]);
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
        case GameStage::RESET:
        
            render_full_scene();

        break;

        case GameStage::GAMEOVER:

            gfx.draw_text
            (
                std::wstring{ L"Game Over\nYour score:\n" } + std::to_wstring(points_counter.get_points()),
                C2,
                GameEngine::DWriteFontNames::VERDANA, 72, 600, WINDOW, 
                GameEngine::DWriteFontStyles::NORMAL,
                GameEngine::DWriteFontStretch::NORMAL,
                GameEngine::DWriteTextHorizontalAlignment::CENTER,
                GameEngine::DWriteTextVerticalAlignment::TOP
            );
            gfx.draw_text
            (
                L"Restart?",
                C1,
                GameEngine::DWriteFontNames::VERDANA, 72, 600, WINDOW,
                GameEngine::DWriteFontStyles::ITALIC,
                GameEngine::DWriteFontStretch::NORMAL,
                GameEngine::DWriteTextHorizontalAlignment::CENTER,
                GameEngine::DWriteTextVerticalAlignment::CENTER
            );
            gfx.draw_text
            (
                L"YES",
                C1,
                GameEngine::DWriteFontNames::VERDANA, 72, 600, YES_BUTTON_AREA,
                GameEngine::DWriteFontStyles::ITALIC,
                GameEngine::DWriteFontStretch::NORMAL,
                GameEngine::DWriteTextHorizontalAlignment::CENTER,
                GameEngine::DWriteTextVerticalAlignment::CENTER
            );
            gfx.draw_text
            (
                L"NO",
                C1,
                GameEngine::DWriteFontNames::VERDANA, 72, 600, NO_BUTTON_AREA,
                GameEngine::DWriteFontStyles::ITALIC,
                GameEngine::DWriteFontStretch::SEMI_EXPANDED,
                GameEngine::DWriteTextHorizontalAlignment::CENTER,
                GameEngine::DWriteTextVerticalAlignment::CENTER
            );
            if      (is_yes_btn_hovered) 
                gfx.draw_rectangle(YES_BUTTON_AREA, BUTTON_BORDER_WIDTH, C1);
            else if (is_no_btn_hovered)  
                gfx.draw_rectangle(NO_BUTTON_AREA, BUTTON_BORDER_WIDTH, C1);

        break;
    }
}
