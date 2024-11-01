#include "PaintItGit.hpp"


PaintItGit& PaintItGit::get_game(HINSTANCE hInstance, int nCmdShow)
{
    static GameEngine::MainWindow window{ hInstance, nCmdShow, GAME_TITLE, RESIZABLE, WINDOW.get_width(), WINDOW.get_height() };
    static GameEngine::GraphicsDirect2D graphics{ window.get_window_handler() };
    static PaintItGit game{ window, graphics };

    return game;
}

PaintItGit& PaintItGit::get_game()
{
    return get_game(NULL, NULL);
}

PaintItGit::PaintItGit(GameEngine::MainWindow& window, GameEngine::GraphicsDirect2D& graphics)
:
Game{ window, graphics },
CURSOR_COLLISION_BOX_WIDTH_HEIGHT{ GameEngine::Mouse::get_cursor_area().get_width_n_height() / 2 },
cursor_pos{ window.get_mouse_pos() },
blocks{ &pulsator, MAIN_COLOURS },
cur_colour_index{ 0U },
prev_colour{ },
cur_block{ blocks.end() },
cur_stage{ GameStage::INIT_COMMIT },
git{ blocks.get_serializer(), blocks.get_deserializer(), get_saver(), get_loader(), N_COLOURS, SAVE_FILE_URI }
{ 
    for (std::size_t i{  }; i != N_BLOCKS_IN_ROW * N_BLOCKS_IN_ROW; ++i)
    {
        origin_grid[i] = (blocks.begin() + i)->get_colour();
    }
    update_available_moves();
}

void PaintItGit::update()
{
    float const dt{ ft.mark() };
    pulsator.update(dt);

    if (cur_input_delay > 0.f) cur_input_delay -= dt;
    colour_pick();

    update_git_pos();

    switch (cur_stage)
    {
        case GameStage::INIT_COMMIT: update_gamestage_first_commit(); break;
        case GameStage::COMMITING:   update_gamestage_commiting();    break;
        case GameStage::ROLLING:     update_gamestage_rolling();      break;
        case GameStage::MERGING:     update_gamestage_merging();      break;
        case GameStage::GAMEOVER:    update_gamestage_gameover();    break;
    }
}

void PaintItGit::update_gamestage_first_commit()
{
    if (COLOUR_FIELD_AREA.contains(cursor_pos) && cur_input_delay < 0.f)
    {
        if (auto const hovered_block{ blocks.get_block(cursor_pos) }; 
            hovered_block->get_colour() != MAIN_COLOURS[cur_colour_index] && 
            get_wnd().is_fun_key_pressed(GameEngine::WinKey::MOUSE_LEFT_BUTTON))
        {
            cur_input_delay = MAX_INPUT_DELAY;

            git.branch(MAIN_COLOURS[cur_colour_index]);
            git.commit(*(cur_block = hovered_block), MAIN_COLOURS[cur_colour_index]);

            for (auto& block : blocks)
            {
                block.pulsation_off();
            }
            cur_stage = GameStage::COMMITING;
            pulsator.reset();
            update_available_moves();
        }
    }
}

void PaintItGit::update_gamestage_commiting()
{
    if (cur_input_delay > 0.f) return;

    if (auto const pressed_key{ get_wnd().get_last_pressed_functional_key() }; pressed_key.has_value())
    {
        cur_input_delay = MAX_INPUT_DELAY;

        switch (pressed_key.value())
        {
            case GameEngine::WinKey::MOUSE_LEFT_BUTTON:
            {
                mlb_on_block_click();

                if (is_all_blocks_one_colour()) cur_stage = GameStage::GAMEOVER;
                update_available_moves();
            }
            break;

            case GameEngine::WinKey::ARROW_UP:
            {
                rollbackward();
                cur_stage = GameStage::ROLLING;
                update_available_moves();
            }
            break;

            case GameEngine::WinKey::SHIFT:
            {
                change_branch();
                update_available_moves();
            }
            break;

            case GameEngine::WinKey::DELETE_:
            {
                delete_branch();
                update_available_moves();
            }
            break;

            case GameEngine::WinKey::ENTER:
            {
                if (git.prepare_merge(MAIN_COLOURS[cur_colour_index]))
                {
                    if (!git.get_conflicts().empty())
                    {
                        for (auto cur{ git.get_conflicts().begin() }; cur != git.get_conflicts().end(); ++cur)
                        {
                            cur_conflicts.emplace_back
                            (
                                DuoColourBlock<decltype(blocks)::BLOCK_SIZE>
                                { 
                                    *reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&cur->get_block()),
                                    true,
                                    cur->get(Option::FIRST),
                                    cur->get(Option::SECOND)
                                }, 
                                cur
                            );
                        }
                        cur_stage = GameStage::MERGING;
                    }
                    else 
                    {
                        cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.merge()));
                        
                        if (is_all_blocks_one_colour()) cur_stage = GameStage::GAMEOVER;
                        else cur_stage = GameStage::COMMITING;
                    }
                    update_available_moves();
                }
            }
            break;
        }
    }
}

void PaintItGit::update_gamestage_rolling()
{
    if (cur_input_delay > 0.f) return;

    if (auto const pressed_key{ get_wnd().get_last_pressed_functional_key() }; pressed_key.has_value())
    {
        cur_input_delay = MAX_INPUT_DELAY;

        switch (pressed_key.value())
        {
            case GameEngine::WinKey::MOUSE_LEFT_BUTTON:
            {
                if (mlb_on_block_click())
                {
                    cur_stage = GameStage::COMMITING;
                    update_available_moves();
                }
            }
            break;

            case GameEngine::WinKey::ARROW_UP:
            {
                rollbackward();
                update_available_moves();
            }
            break;

            case GameEngine::WinKey::ARROW_DOWN:
            {
                rollforward();

                if (!git.is_behind_head()) cur_stage = GameStage::COMMITING;

                update_available_moves();
            }
            break;

            case GameEngine::WinKey::SHIFT:
            {
                if (change_branch()) cur_stage = GameStage::COMMITING;

                update_available_moves();
            }
            break;

            case GameEngine::WinKey::DELETE_:
            {
                delete_branch();
                update_available_moves();
            }
            break;
        }
    }
}

void PaintItGit::update_gamestage_merging()
{
    if (std::ranges::all_of(cur_conflicts, [](auto const& pair){ return pair.first.is_option_set(); }))
    {
        for (auto const& pair : cur_conflicts)
        {
            pair.second->set_option(pair.first.get_option());
        }
        cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.merge()));
        
        cur_conflicts.clear();
        if (is_all_blocks_one_colour()) cur_stage = GameStage::GAMEOVER;
        else cur_stage = GameStage::COMMITING;
        update_available_moves();

        return;
    }

    if (cur_input_delay > 0.f) return;

    if (auto const pressed_key{ get_wnd().get_last_pressed_functional_key() }; pressed_key.has_value())
    {
        cur_input_delay = MAX_INPUT_DELAY;

        switch (pressed_key.value())
        {
            case GameEngine::WinKey::MOUSE_LEFT_BUTTON:
            {
                for (auto& [block, dummy] : cur_conflicts | std::views::filter([](auto const& pair){ return !pair.first.is_option_set(); }))
                {
                    block.choose_colour(cursor_pos);
                }
            }
            break;

            case GameEngine::WinKey::BACKSPACE:
            {
                for (auto& [block, dummy] : cur_conflicts)
                {
                    block.set_option(Option::NONE);
                }
            }
            break;
        }
    }
}

void PaintItGit::update_gamestage_gameover()
{
    
}

void PaintItGit::update_available_moves()
{
    switch (cur_stage)
    {
        case GameStage::INIT_COMMIT:
        {
            for (auto& block : blocks)
            {
                if (block.get_colour() == MAIN_COLOURS[cur_colour_index]) block.pulsation_off();
                else block.pulsation_on();
            }
        }
        break;

        case GameStage::COMMITING:
        {
            unset_pulsation();
            if (MAIN_COLOURS[cur_colour_index] != git.get_cur_branch() && git.has_branch(MAIN_COLOURS[cur_colour_index]))
                n_available_adject_cur_blocks = 0U;
            else
                find_adject_blocks();
            set_pulsation();
        }
        break;

        case GameStage::ROLLING:
        {
            unset_pulsation();
            if (MAIN_COLOURS[cur_colour_index] == git.get_cur_branch() || (MAIN_COLOURS[cur_colour_index] != git.get_cur_branch() && git.has_branch(MAIN_COLOURS[cur_colour_index])))
                n_available_adject_cur_blocks = 0U;
            else
                find_adject_blocks();
            set_pulsation();
        }
        break;

        case GameStage::GAMEOVER:
        {
            unset_pulsation();
            n_available_adject_cur_blocks = 0U;
        }
        break;
    }
}

bool PaintItGit::check_mouse_wheel()
{
    if (int const mouse_wheel_rotation_destance{ get_wnd().get_mouse_wheel_rotation_destance() }; mouse_wheel_rotation_destance > 0)
    {
        select_next_colour();
        return true;
    }
    else if (mouse_wheel_rotation_destance < 0)
    {
        select_prev_colour();
        return true;
    }
    return false;
}

void PaintItGit::update_git_pos()
{
    static Vec2i prev_cursor_pos{ cursor_pos };

    if (GIT_COLOUR_AREA.contains(cursor_pos) && get_wnd().is_fun_key_pressed(GameEngine::WinKey::MOUSE_LEFT_BUTTON))
        git.move_in_frame(cursor_pos - prev_cursor_pos);
    
    prev_cursor_pos = cursor_pos;
}

bool PaintItGit::change_branch()
{
    if (MAIN_COLOURS[cur_colour_index] != git.get_cur_branch() && git.has_branch(MAIN_COLOURS[cur_colour_index]))
    {
        cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.checkout(MAIN_COLOURS[cur_colour_index])));

        return true;
    }
    else if (MAIN_COLOURS[cur_colour_index] == git.get_cur_branch())
    {
        cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.move_to_head()));

        return true;
    }

    return false;
}

void PaintItGit::rollbackward()
{
    cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.rollback()));
}

void PaintItGit::rollforward()
{
    cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.rollforward()));
}

bool PaintItGit::mlb_on_block_click()
{
    if (!COLOUR_FIELD_AREA.contains(cursor_pos)) return false;

    if (auto const hovered_block{ blocks.get_block(cursor_pos) };
        std::ranges::find(adject_cur_blocks | std::views::take(n_available_adject_cur_blocks), hovered_block) != adject_cur_blocks.begin() + n_available_adject_cur_blocks)
    {
        if (MAIN_COLOURS[cur_colour_index] != git.get_cur_branch()) git.branch(MAIN_COLOURS[cur_colour_index]);
        git.commit(*(cur_block = hovered_block), MAIN_COLOURS[cur_colour_index]);

        return true;
    }
    return false;
}

void PaintItGit::unset_pulsation()
{
    std::ranges::for_each_n(adject_cur_blocks.begin(), n_available_adject_cur_blocks, [](auto& block) { block->pulsation_off(); });
}

void PaintItGit::set_pulsation()
{
    std::ranges::for_each_n(adject_cur_blocks.begin(), n_available_adject_cur_blocks, [](auto& block) { block->pulsation_on(); });
}

void PaintItGit::find_adject_blocks()
{
    n_available_adject_cur_blocks = blocks.get_adject_blocks(cur_block, adject_cur_blocks,
        [&main_colours = this->MAIN_COLOURS, &main_colour_index = this->cur_colour_index](GameEngine::Colour c)
        {
            return c != main_colours[main_colour_index];
        });
}

void PaintItGit::delete_branch()
{
    auto const new_state{ git.delete_branch(MAIN_COLOURS[cur_colour_index]) };
    if (!new_state) cur_stage = GameStage::INIT_COMMIT;
    else
    {
        for (std::size_t i{ 0U }; i != N_COLOURS; ++i)
        {
            if (MAIN_COLOURS[i] == new_state->first)
            {
                cur_colour_index = i;
                break;
            }
        }
        cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(new_state->second));
    }
}

void PaintItGit::colour_pick()
{
    if (cur_input_delay > 0.f) return;

    if (check_mouse_wheel()) update_available_moves();
    else if (auto const pressed_key{ get_wnd().get_last_pressed_non_functional_key() }; pressed_key.has_value())
    {
        cur_input_delay = MAX_INPUT_DELAY;

        switch (pressed_key.value())
        {
            case '1':
            {
                prev_colour = MAIN_COLOURS[cur_colour_index];
                cur_colour_index = 0U;

                update_available_moves();
            }
            break;

            case '2':
            {
                prev_colour = MAIN_COLOURS[cur_colour_index];
                cur_colour_index = 1U;

                update_available_moves();
            }
            break;

            case '3':
            {
                prev_colour = MAIN_COLOURS[cur_colour_index];
                cur_colour_index = 2U;

                update_available_moves();
            }
            break;

            case '4':
            {
                prev_colour = MAIN_COLOURS[cur_colour_index];
                cur_colour_index = 3U;

                update_available_moves();
            }
            break;

            case '5':
            {
                prev_colour = MAIN_COLOURS[cur_colour_index];
                cur_colour_index = 4U;

                update_available_moves();
            }
            break;

            case 33:
            {
                select_next_colour();
                update_available_moves();
            }
            break;

            case 34:
            {
                select_prev_colour();
                update_available_moves();
            }
            break;
        }
    }
}

void PaintItGit::select_next_colour()
{
    prev_colour = MAIN_COLOURS[cur_colour_index];
    cur_colour_index = (cur_colour_index + 1U) % MAIN_COLOURS.size();
}

void PaintItGit::select_prev_colour()
{
    prev_colour = MAIN_COLOURS[cur_colour_index];
    cur_colour_index = (cur_colour_index == 0U ? MAIN_COLOURS.size() - 1U : cur_colour_index - 1U);
}

std::function<void(std::ofstream&)> PaintItGit::get_saver()
{
    assert(cur_conflicts.empty());
    return [this](std::ofstream& fout)
        {
            fout.write(reinterpret_cast<char const*>(&cur_stage), sizeof(cur_stage));
            auto tmp1{ std::distance(blocks.begin(), cur_block) };
            fout.write(reinterpret_cast<char const*>(&tmp1), sizeof(tmp1));
            fout.write(reinterpret_cast<char const*>(&cur_colour_index), sizeof(cur_colour_index));
            fout.write(reinterpret_cast<char const*>(&prev_colour), sizeof(prev_colour));

            for (auto const& block : origin_grid)
            {
                auto tmp2{ block.rgba };

                fout.write(reinterpret_cast<char const*>(&tmp2), sizeof(tmp2));
            }

            fout.flush();
        };
}

std::function<void(std::ifstream&)> PaintItGit::get_loader()
{
    return [this](std::ifstream& fin)
        {
            int saved_stage{ };
            ptrdiff_t saved_distance{ };
            std::size_t saved_cur_colour_index{ };
            std::size_t saved_n_available{ };
            GameEngine::Colour saved_prev_c{ };

            fin.read(reinterpret_cast<char*>(&saved_stage), sizeof(saved_stage));
            fin.read(reinterpret_cast<char*>(&saved_distance), sizeof(saved_distance));
            fin.read(reinterpret_cast<char*>(&saved_cur_colour_index), sizeof(saved_cur_colour_index));
            fin.read(reinterpret_cast<char*>(&saved_prev_c), sizeof(saved_prev_c));

            cur_stage = static_cast<GameStage>(saved_stage);
            cur_block = blocks.begin() + saved_distance;
            cur_colour_index = saved_cur_colour_index;
            prev_colour = saved_prev_c;

            for (auto& block : blocks)
            {
                GameEngine::Colour c{ };

                fin.read(reinterpret_cast<char*>(&c.rgba), sizeof(c.rgba));

                block.set_colour(c);
            }
        };
}

constexpr bool PaintItGit::is_all_blocks_one_colour() const noexcept
{
    return std::ranges::all_of(blocks, [c = cur_block->get_colour()](auto const& block){ return block.get_colour() == c; });
}

void PaintItGit::render()
{
    gfx.draw_rectangle(COLOUR_FIELD_AREA, INNER_BORDER_THICKNESS, INNER_BORDER_C);
    gfx.draw_rectangle(COLOUR_FIELD_AREA.get_expanded(INNER_BORDER_THICKNESS), OUTER_BORDER_THICKNESS, OUTER_BORDER_C);
    blocks.draw(gfx);

    for (auto const& conflict : cur_conflicts)
    {
        conflict.first.draw(gfx);
    }

    gfx.draw_rectangle(GIT_COLOUR_AREA, INNER_BORDER_THICKNESS, INNER_BORDER_C);
    gfx.draw_rectangle(GIT_COLOUR_AREA.get_expanded(INNER_BORDER_THICKNESS), OUTER_BORDER_THICKNESS, OUTER_BORDER_C);
    git.draw(gfx);

    if (cur_stage != GameStage::MERGING && cur_stage != GameStage::GAMEOVER)
    { 
        Rec2i const miniature_area{ cursor_pos + CURSOR_COLLISION_BOX_WIDTH_HEIGHT, MINIATURE_SIZE, MINIATURE_SIZE};
        gfx.draw_rectangle(miniature_area, MINIATURE_STROKE_WIDTH, INNER_BORDER_C);
        gfx.draw_rectangle(miniature_area.get_expanded(MINIATURE_STROKE_WIDTH), MINIATURE_STROKE_WIDTH, OUTER_BORDER_C);
        gfx.fill_rectangle(miniature_area, MAIN_COLOURS[cur_colour_index]);
    }
    if (cur_stage == GameStage::GAMEOVER)
    {
        gfx.draw_text
        (
            GAMEOVER_T,
            GAMEOVER_C,
            GameEngine::DWriteFontNames::VERDANA, 72, 600, WINDOW,
            GameEngine::DWriteFontStyles::NORMAL,
            GameEngine::DWriteFontStretch::NORMAL,
            GameEngine::DWriteTextHorizontalAlignment::CENTER,
            GameEngine::DWriteTextVerticalAlignment::CENTER
        );
    }
}