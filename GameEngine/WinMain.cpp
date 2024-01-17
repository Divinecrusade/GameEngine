#include "Game.hpp"
#include "MainWindow/MainWindow.hpp"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    MainWindow& m_wnd{ MainWindow::instance(hInstance, nCmdShow, L"Test App", 400U, 500U) };
    Game theGame{ m_wnd };
    theGame.start();

    return EXIT_SUCCESS;
}