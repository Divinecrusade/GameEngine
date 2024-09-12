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
cursor_pos{ window.get_mouse_pos() }
{
    update_available_moves();
}

void PaintItGit::update()
{
    float const dt{ ft.mark() };
    pulsator.update(dt);

    bool mouse_rotated{ false };
    if (int const mouse_wheel_rotation_destance{ get_wnd().get_mouse_wheel_rotation_destance() }; mouse_wheel_rotation_destance > 0)
    {
        cur_colour_index = (cur_colour_index + 1U) % MAIN_COLOURS.size();
        mouse_rotated = true;
    }
    else if (mouse_wheel_rotation_destance < 0)
    {
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
    if (COLOUR_FIELD_AREA.contains(cursor_pos))
    {
        if (auto const hovered_block{ blocks.get_block(cursor_pos) }; (*hovered_block).first != MAIN_COLOURS[cur_colour_index] && get_wnd().is_fun_key_pressed(GameEngine::WinKey::MOUSE_LEFT_BUTTON))
        {
            (*hovered_block).first = MAIN_COLOURS[cur_colour_index];
            cur_block = hovered_block;

            pulsator.reset();
            cur_stage = GameStage::COMMITING;
            update_available_moves();
        }
    }
}

void PaintItGit::update_gamestage_commiting()
{
    if (COLOUR_FIELD_AREA.contains(cursor_pos) && get_wnd().is_fun_key_pressed(GameEngine::WinKey::MOUSE_LEFT_BUTTON))
    {
        auto const hovered_block{ blocks.get_block(cursor_pos) };
        bool available_move{ false };
        for (auto cur{ adject_cur_blocks.begin() }; cur != adject_cur_blocks.begin() + n_adject_cur_blocks_with_diff_colours; ++cur)
        {
            if (*cur == hovered_block)
            {
                available_move = true;
                break;
            }
        }
        if (available_move)
        {
            (*hovered_block).first = MAIN_COLOURS[cur_colour_index];
            cur_block = hovered_block;
            pulsator.reset();
            update_available_moves();
        }
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
                if (block.first != MAIN_COLOURS[cur_colour_index]) block.second = pulsation;
                else                                               block.second = std::nullopt;
            }
        }
        break;

        case GameStage::COMMITING:
        {
            for (auto& block : blocks)
            {
                block.second = std::nullopt;
            }
            n_adject_cur_blocks_with_diff_colours = blocks.get_adject_blocks_with_not_equal_color(cur_block, MAIN_COLOURS[cur_colour_index], adject_cur_blocks);
            for (auto cur{ adject_cur_blocks.begin() }; cur != adject_cur_blocks.begin() + n_adject_cur_blocks_with_diff_colours; ++cur)
            {
                (*cur)->second = pulsation;
            }
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
}