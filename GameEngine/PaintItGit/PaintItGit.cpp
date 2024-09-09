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
    switch (cur_stage)
    {
        case GameStage::INIT_COMMIT: update_init_commit(); break;
        case GameStage::COMITTING:   break;
    }
}

void PaintItGit::update_init_commit()
{
    if (Vec2i const mouse_pos{ GameEngine::Mouse::get_cursor_pos(get_wnd()) }; COLOR_FIELD_AREA.contains(mouse_pos))
    {
        hovered = true;
        cursor_pos = mouse_pos;
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