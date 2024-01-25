#include "Game.hpp"
#include "MainWindow/MainWindow.hpp"
#include "GraphicsDirect2D.hpp"


using namespace GameEngine;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    MainWindow& m_wnd{ MainWindow::instance(hInstance, nCmdShow, L"Test App") };
    GraphicsDirect2D graphics{ m_wnd.get_window_handler() };
    Game theGame{ m_wnd, graphics };
    theGame.start();

    return EXIT_SUCCESS;
}