#include "MainWindow.hpp"
#include <stdexcept>


MainWindow& MainWindow::instance(HINSTANCE _In_ hInstance, int _In_ nCmdShow, std::wstring_view window_name, size_t init_width, size_t init_height)
{
    static MainWindow wnd{ hInstance, nCmdShow, window_name, init_width, init_height };

    return wnd;
}

MainWindow::MainWindow(HINSTANCE _In_ hInstance, int _In_ nCmdShow, std::wstring_view window_name, size_t init_width, size_t init_height)
:
H_INSTANCE{ hInstance },
WND_TITLE{ window_name }
{
    WNDCLASSEX const wcex
    {
        sizeof(wcex),
        CS_HREDRAW | CS_VREDRAW,
        MainWindow::message_handler,
        0,
        0,
        hInstance,
        LoadIcon(hInstance, IDI_APPLICATION),
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        MainWindow::WND_CLASS_NAME,
        LoadIcon(hInstance, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&wcex))
    {
        throw std::runtime_error{ "Failed to register class" };
    }

    HWND const hWnd
    {
        CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,
            MainWindow::WND_CLASS_NAME,
            WND_TITLE.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            init_width, init_height,
            NULL,
            NULL,
            hInstance,
            NULL
        )
    };
    if (!hWnd)
    {
        UnregisterClass(MainWindow::WND_CLASS_NAME, hInstance);

        throw std::runtime_error{ "Failed to create window" };
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
}

MainWindow::~MainWindow()
{
    UnregisterClass(MainWindow::WND_CLASS_NAME, H_INSTANCE);
    PostQuitMessage(EXIT_SUCCESS);
}

LRESULT MainWindow::message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    PAINTSTRUCT ps{ };
    HDC hdc{ };
    wchar_t const* greeting = L"Hello, Windows desktop!";

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOutW(hdc, 5, 5, greeting, wcslen(greeting));

        EndPaint(hWnd, &ps);

        break;

    case WM_DESTROY:
        PostQuitMessage(EXIT_SUCCESS);
        break;

    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
        break;
    }

    return EXIT_SUCCESS;
}
