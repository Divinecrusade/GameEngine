#include "Arkanoid.hpp"
#include "../MainWindow/MainWindow.hpp"
#include "../GraphicsDirect2D.hpp"


using namespace GameEngine2D;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    MainWindow& m_wnd{ MainWindow::instance(hInstance, nCmdShow, L"Arkanoid") };
    GraphicsDirect2D graphics{ m_wnd.get_window_handler() };
    Arkanoid theGame{ m_wnd, graphics };
    theGame.start();

    return EXIT_SUCCESS;
}