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
blocks{ &pulsator, MAIN_COLOURS, cur_colour_index },
git{ N_COLOURS }
{ }

void PaintItGit::update()
{
    float const dt{ ft.mark() };
    pulsator.update(dt);

    if (cur_input_delay > 0.f) cur_input_delay -= dt;

    bool mouse_rotated{ false };
    if (int const mouse_wheel_rotation_destance{ get_wnd().get_mouse_wheel_rotation_destance() }; mouse_wheel_rotation_destance > 0)
    {
        prev_colour = MAIN_COLOURS[cur_colour_index];
        cur_colour_index = (cur_colour_index + 1U) % MAIN_COLOURS.size();
        mouse_rotated = true;
    }
    else if (mouse_wheel_rotation_destance < 0)
    {
        prev_colour = MAIN_COLOURS[cur_colour_index];
        cur_colour_index = (cur_colour_index == 0U ? MAIN_COLOURS.size() - 1U : cur_colour_index - 1U);
        mouse_rotated = true;
    }

    if (mouse_rotated) update_available_moves();

    switch (cur_stage)
    {
        case GameStage::INIT_COMMIT: update_gamestage_first_commit(); break;
        case GameStage::COMMITING:   update_gamestage_commiting();    break;
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

    auto const pressed_key{ get_wnd().get_last_pressed_functional_key() };

    if (pressed_key.has_value())
    switch (pressed_key.value())
    {
        case GameEngine::WinKey::MOUSE_LEFT_BUTTON:
        {
            if (!COLOUR_FIELD_AREA.contains(cursor_pos)) break;

            cur_input_delay = MAX_INPUT_DELAY;

            if (auto const hovered_block{ blocks.get_block(cursor_pos) };
                std::ranges::find(adject_cur_blocks | std::views::take(n_available_adject_cur_blocks), hovered_block) != adject_cur_blocks.begin() + n_available_adject_cur_blocks)
            {
                if (MAIN_COLOURS[cur_colour_index] != git.get_cur_branch()) git.branch(MAIN_COLOURS[cur_colour_index]);

                git.commit(*(cur_block = hovered_block), MAIN_COLOURS[cur_colour_index]);

                pulsator.reset();
                update_available_moves();
            }
        }
        break;

        case GameEngine::WinKey::ARROW_UP:
        {
            cur_input_delay = MAX_INPUT_DELAY;

            cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.rollback()));
            pulsator.reset();
            update_available_moves();
        }
        break;

        case GameEngine::WinKey::ARROW_DOWN:
        {
            cur_input_delay = MAX_INPUT_DELAY;

            cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.rollforward()));
            pulsator.reset();
            update_available_moves();
        }
        break;

        case GameEngine::WinKey::SHIFT:
        {
            cur_input_delay = MAX_INPUT_DELAY;

            if (MAIN_COLOURS[cur_colour_index] != git.get_cur_branch() && git.has_branch(MAIN_COLOURS[cur_colour_index]))
            {
                cur_block = blocks.get_iterator(reinterpret_cast<PulsatingBlock<decltype(blocks)::BLOCK_SIZE>*>(&git.checkout(MAIN_COLOURS[cur_colour_index])));
                pulsator.reset();
                update_available_moves();
            }
        }
        break;
    }
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
                else if (block.get_colour() == prev_colour) block.pulsation_on();
            }
        }
        break;

        case GameStage::COMMITING:
        {
            std::ranges::for_each_n(adject_cur_blocks.begin(), n_available_adject_cur_blocks, [](auto& block) { block->pulsation_off(); });
            if (MAIN_COLOURS[cur_colour_index] != git.get_cur_branch() && git.has_branch(MAIN_COLOURS[cur_colour_index])) 
                n_available_adject_cur_blocks = 0U;
            else
                n_available_adject_cur_blocks = blocks.get_adject_blocks(cur_block, adject_cur_blocks, 
                [&main_colours = this->MAIN_COLOURS, &main_colour_index = this->cur_colour_index](GameEngine::Colour c)
                { 
                    return c != main_colours[main_colour_index];
                });
            std::ranges::for_each_n(adject_cur_blocks.begin(), n_available_adject_cur_blocks, [](auto& block) { block->pulsation_on(); });
        }
        break;
    }
}

void PaintItGit::render()
{
    gfx.draw_rectangle(COLOUR_FIELD_AREA, INNER_BORDER_THICKNESS, INNER_BORDER_C);
    gfx.draw_rectangle(COLOUR_FIELD_AREA.get_expanded(INNER_BORDER_THICKNESS), OUTER_BORDER_THICKNESS, OUTER_BORDER_C);
    blocks.draw(gfx);

    Rec2i const miniature_area{ cursor_pos + CURSOR_COLLISION_BOX_WIDTH_HEIGHT, MINIATURE_SIZE, MINIATURE_SIZE};
    gfx.draw_rectangle(miniature_area, MINIATURE_STROKE_WIDTH, INNER_BORDER_C);
    gfx.draw_rectangle(miniature_area.get_expanded(MINIATURE_STROKE_WIDTH), MINIATURE_STROKE_WIDTH, OUTER_BORDER_C);
    gfx.fill_rectangle(miniature_area, MAIN_COLOURS[cur_colour_index]);

    git.draw(gfx, std::nullopt);
}