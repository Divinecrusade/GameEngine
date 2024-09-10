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
{ }

void PaintItGit::update()
{
    float const dt{ ft.mark() };
    switch (cur_stage)
    {
        case GameStage::INIT_COMMIT: update_gamestage_first_commit(dt); break;
        case GameStage::COMMITING:   update_gamestage_commiting();    break;
    }
}

void PaintItGit::update_gamestage_first_commit(float dt)
{
    static constexpr float MAX_INPUT_DELAY{ 0.15f };
    static float cur_input_delay{ -MAX_INPUT_DELAY };

    if (cur_input_delay > 0.f) cur_input_delay -= dt;

    if (COLOUR_FIELD_AREA.contains(cursor_pos))
    {
        blocks_hovered = true;

        if (int const mouse_wheel_rotation_destance{ get_wnd().get_mouse_wheel_rotation_destance() }; mouse_wheel_rotation_destance > 0)
            cur_colour_index = (cur_colour_index + 1U) % MAIN_COLOURS.size();
        else if (mouse_wheel_rotation_destance < 0)
            cur_colour_index = (cur_colour_index == 0U ? MAIN_COLOURS.size() - 1U : cur_colour_index - 1U);

        if (cur_input_delay < 0.f && get_wnd().is_fun_key_pressed(GameEngine::WinKey::MOUSE_LEFT_BUTTON))
            *blocks.get_block(cursor_pos) = MAIN_COLOURS[cur_colour_index];
    }
    else blocks_hovered = false;
}

void PaintItGit::update_gamestage_commiting()
{

}

void PaintItGit::render()
{
    gfx.draw_rectangle(COLOUR_FIELD_AREA, INNER_BORDER_THICKNESS, INNER_BORDER_C);
    gfx.draw_rectangle(COLOUR_FIELD_AREA.get_expanded(INNER_BORDER_THICKNESS), OUTER_BORDER_THICKNESS, OUTER_BORDER_C);
    blocks.draw(gfx);

    if (blocks_hovered)
    {
        Rec2i const miniature_area{ cursor_pos + CURSOR_COLLISION_BOX_WIDTH_HEIGHT, MINIATURE_SIZE, MINIATURE_SIZE};
        gfx.draw_rectangle(miniature_area, MINIATURE_STROKE_WIDTH, INNER_BORDER_C);
        gfx.draw_rectangle(miniature_area.get_expanded(MINIATURE_STROKE_WIDTH), MINIATURE_STROKE_WIDTH, OUTER_BORDER_C);
        gfx.fill_rectangle(miniature_area, MAIN_COLOURS[cur_colour_index]);
    }
}