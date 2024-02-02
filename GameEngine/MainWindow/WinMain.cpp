#include "MainWindow.hpp"

using namespace GameEngine;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow)
{
    MainWindow& m_wnd{ MainWindow::instance(hInstance, nCmdShow, L"Test App", true, 400, 500) };

    return (int) m_wnd.start_message_loop().wParam;
}