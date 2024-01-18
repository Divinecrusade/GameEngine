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
    DestroyWindow(H_WND);
    UnregisterClassW(MainWindow::WND_CLASS_NAME, H_INSTANCE);
}

LRESULT MainWindow::message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept
{
    switch (message)
    {
    case WM_CLOSE:
    case WM_DESTROY:

        PostQuitMessage(EXIT_SUCCESS);

        MainWindow::instance().terminated = true;

        break;

    default:
        break;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

HWND MainWindow::register_and_create_window(HINSTANCE hInstance, std::wstring const& window_name, size_t init_width, size_t init_height)
{
    if (hInstance == NULL) hInstance = (HINSTANCE) GetModuleHandleW(NULL);

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
        (HBRUSH) GetStockObject(BLACK_BRUSH),
        NULL,
        MainWindow::WND_CLASS_NAME,
        LoadIconW(hInstance, IDI_APPLICATION)
    };

    if (!RegisterClassExW(&wcex))
    {
        DWORD const last_error{ GetLastError() };
        // TODO: make exception, that obtain error code
        if (last_error != ERROR_CLASS_ALREADY_EXISTS)
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
            static_cast<int>(init_width), static_cast<int>(init_height),
            NULL,
            NULL,
            hInstance,
            NULL
        )
    };
    if (!hWnd)
    {
        UnregisterClassW(MainWindow::WND_CLASS_NAME, hInstance);

        // TODO: make exception, that obtain error code
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
    }
}

bool MainWindow::is_terminated() const noexcept
{
    return terminated;
}
