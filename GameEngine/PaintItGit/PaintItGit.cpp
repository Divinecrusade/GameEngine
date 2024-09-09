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
Game{ window, graphics }
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

    cur_input_delay -= dt;

    if (Vec2i const mouse_pos{ get_wnd().get_mouse_pos()}; COLOR_FIELD_AREA.contains(mouse_pos))
    {
        hovered = true;
        cursor_pos = mouse_pos;

        if (cur_input_delay < 0.f)
        {
            if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_UP))
            {
                cur_color_index = (cur_color_index + 1U) % MAIN_COLOURS.size();
                cur_input_delay = MAX_INPUT_DELAY;
            }
            else if (get_wnd().is_fun_key_pressed(GameEngine::WinKey::ARROW_DOWN))
            {
                cur_color_index = (cur_color_index == 0U ? MAIN_COLOURS.size() - 1U : cur_color_index - 1U);
                cur_input_delay = MAX_INPUT_DELAY;
            }
        }
    }
    else hovered = false;
}

void PaintItGit::render()
{
    blocks.draw(gfx);

    if (hovered)
    {
        Rec2i const miniature_area{ cursor_pos, MINIATURE_SIZE, MINIATURE_SIZE };
        gfx.draw_rectangle(miniature_area.get_expanded(MINIATURE_STROKE_WIDTH), MINIATURE_STROKE_WIDTH, MINIATURE_C);
        gfx.fill_rectangle(miniature_area, MAIN_COLOURS[cur_color_index]);
    }
}