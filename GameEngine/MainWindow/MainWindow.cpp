#include "MainWindow.hpp"
#include <stdexcept>


MainWindow& MainWindow::instance(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, size_t init_width, size_t init_height)
{
    static MainWindow wnd{ hInstance, nCmdShow, window_name, init_width, init_height };

    return wnd;
}

MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, size_t init_width, size_t init_height)
:
H_INSTANCE{ hInstance   },
WND_TITLE { window_name },
H_WND     { MainWindow::register_and_create_window(H_INSTANCE, WND_TITLE, init_width, init_height)   }
{
    ShowWindow(H_WND, nCmdShow);
    UpdateWindow(H_WND);
}

MainWindow::~MainWindow()
{
    UnregisterClassW(MainWindow::WND_CLASS_NAME, H_INSTANCE);
    PostQuitMessage(EXIT_SUCCESS);
}

LRESULT MainWindow::message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept
{
    PAINTSTRUCT ps{ };
    HDC hdc{ };

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

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

HWND MainWindow::register_and_create_window(HINSTANCE hInstance, std::wstring const& window_name, size_t init_width, size_t init_height)
{
    WNDCLASSEX const wcex
    {
        sizeof(wcex),
        CS_HREDRAW | CS_VREDRAW,
        MainWindow::message_handler,
        0,
        0,
        hInstance,
        LoadIconW(hInstance, IDI_APPLICATION),
        LoadCursorW(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        MainWindow::WND_CLASS_NAME,
        LoadIconW(hInstance, IDI_APPLICATION)
    };

    if (!RegisterClassExW(&wcex))
    {
        throw std::runtime_error{ "Failed to register class" };
    }

    HWND const hWnd
    {
        CreateWindowExW(
            WS_EX_OVERLAPPEDWINDOW,
            MainWindow::WND_CLASS_NAME,
            window_name.c_str(),
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
        UnregisterClassW(MainWindow::WND_CLASS_NAME, hInstance);

        throw std::runtime_error{ "Failed to create window" };
    }

    return hWnd;
}

void MainWindow::process_messages_queue() noexcept
{
    MSG msg{ };
    while (PeekMessageW(&msg, H_WND, 0U, 0U, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        terminated = msg.message == WM_QUIT;
    }
}

bool MainWindow::is_terminated() const noexcept
{
    return terminated;
}
