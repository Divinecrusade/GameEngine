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
    update_cursor(dt);
    switch (cur_stage)
    {
        case GameStage::INIT_COMMIT: break;
        case GameStage::COMITTING:   break;
    }
}

void PaintItGit::update_cursor(float dt)
{
    static constexpr float MAX_INPUT_DELAY{ 0.15f };
    static float cur_input_delay{ 0.f };

    if (COLOR_FIELD_AREA.contains(cursor_pos))
    {
        hovered = true;

        if (int const mouse_wheel_rotation_destance{ get_wnd().get_mouse_wheel_rotation_destance() }; mouse_wheel_rotation_destance > 0)
        {
            cur_color_index = (cur_color_index + 1U) % MAIN_COLOURS.size();
        }
        else if (mouse_wheel_rotation_destance < 0)
        {
            cur_color_index = (cur_color_index == 0U ? MAIN_COLOURS.size() - 1U : cur_color_index - 1U);
        }
    }
    else hovered = false;
}

void PaintItGit::render()
{
    gfx.draw_rectangle(COLOR_FIELD_AREA.get_expanded(INNER_BORDER_THICKNESS), INNER_BORDER_THICKNESS, INNER_BORDER_C);
    gfx.draw_rectangle(COLOR_FIELD_AREA.get_expanded(INNER_BORDER_THICKNESS + OUTER_BORDER_THICKNESS), OUTER_BORDER_THICKNESS, OUTER_BORDER_C);
    blocks.draw(gfx);

    if (hovered)
    {
        Rec2i const miniature_area{ cursor_pos + CURSOR_COLLISION_BOX_WIDTH_HEIGHT, MINIATURE_SIZE, MINIATURE_SIZE};
        gfx.draw_rectangle(miniature_area.get_expanded(MINIATURE_STROKE_WIDTH), MINIATURE_STROKE_WIDTH, MINIATURE_C);
        gfx.fill_rectangle(miniature_area, MAIN_COLOURS[cur_color_index]);
    }
}