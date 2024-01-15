#include "MainWindow.hpp"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    MainWindow& m_wnd{ MainWindow::instance(hInstance, nCmdShow, L"Test App", 400U, 500U) };
    MainWindow& test{ MainWindow::instance() };

    MSG msg{ };
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int) msg.wParam;
}