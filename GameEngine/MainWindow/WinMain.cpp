#include "MainWindow.hpp"

using namespace GameEngine2D;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    MainWindow& m_wnd{ MainWindow::instance(hInstance, nCmdShow, L"Test App", 400U, 500U) };

    return (int) m_wnd.start_message_loop().wParam;
}