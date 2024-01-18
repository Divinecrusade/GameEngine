#include "MainWindow.hpp"
#include <stdexcept>
#include <cassert>


MainWindow& MainWindow::instance(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, int init_width, int init_height)
{
    static MainWindow wnd{ hInstance, nCmdShow, window_name, init_width, init_height };

    return wnd;
}

MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, int init_width, int init_height)
:
H_INSTANCE{ hInstance == NULL ? (HINSTANCE)GetModuleHandleW(NULL) : hInstance },
WND_TITLE { window_name },
H_WND     { MainWindow::register_and_create_window(H_INSTANCE, WND_TITLE, init_width, init_height)   }
{
    assert(H_WND);

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
    static constexpr int MESSAGE_HANDLED{ 0 };

    switch (message)
    {
    case WM_DISPLAYCHANGE:
    
        InvalidateRect(hWnd, NULL, FALSE);
        
        break;

    case WM_PAINT:

        ValidateRect(hWnd, NULL);

        break;

    case WM_DESTROY:

        PostQuitMessage(EXIT_SUCCESS);

        MainWindow::instance().terminated = true;

        break;

    default:
        
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }

    return MESSAGE_HANDLED;
}

HWND MainWindow::register_and_create_window(HINSTANCE hInstance, std::wstring const& window_name, int init_width, int init_height)
{
    assert(hInstance != NULL);
    assert(init_width > 0);
    assert(init_height > 0);

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

        // TODO: make exception, that obtain error code
        throw std::runtime_error{ "Failed to create window" };
    }
    unsigned const dpi { GetDpiForWindow(hWnd) };
    SetWindowPos(
        hWnd,
        NULL,
        NULL,
        NULL,
        init_width * dpi / USER_DEFAULT_SCREEN_DPI,
        init_height * dpi / USER_DEFAULT_SCREEN_DPI,
        SWP_NOMOVE);

    return hWnd;
}

HWND MainWindow::get_window_handler() const noexcept
{
    return H_WND;
}

void MainWindow::process_messages_queue() noexcept
{
    assert(!is_terminated());

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
