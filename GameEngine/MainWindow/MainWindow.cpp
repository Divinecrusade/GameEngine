#include "MainWindow.hpp"
#include <stdexcept>
#include <cassert>


namespace GameEngine2D
{
    MainWindow& MainWindow::instance(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, bool resizable, int init_width, int init_height)
    {
        static MainWindow wnd{ hInstance, nCmdShow, window_name, resizable, init_width, init_height };

        return wnd;
    }

    MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, bool resizable, int init_width, int init_height)
    :
    H_INSTANCE{ hInstance == NULL ? (HINSTANCE)GetModuleHandleW(NULL) : hInstance },
    WND_TITLE { window_name },
    H_WND     { MainWindow::register_and_create_window(H_INSTANCE, WND_TITLE, resizable, init_width, init_height)   }
    {
        assert(H_WND);

        ShowWindow(H_WND, nCmdShow);
        UpdateWindow(H_WND);
    }

    MainWindow::~MainWindow()
    {
        if (!terminated) DestroyWindow(H_WND);
        UnregisterClassW(MainWindow::WND_CLASS_NAME, H_INSTANCE);
    }

    LRESULT MainWindow::message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept
    {
        static constexpr int MESSAGE_HANDLED{ 0 };

        switch (message)
        {
        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_LBUTTON:
                case VK_RBUTTON:
                case VK_MBUTTON:
                case VK_CANCEL:
                case VK_CONTROL:
                case VK_LCONTROL:
                case VK_RCONTROL:
                case VK_BACK:
                case VK_TAB:
                case VK_RETURN:
                case VK_SHIFT:
                case VK_LSHIFT:
                case VK_RSHIFT:
                case VK_MENU:
                case VK_LMENU:
                case VK_RMENU:
                case VK_CAPITAL:
                case VK_SPACE:
                case VK_LEFT:
                case VK_RIGHT:
                case VK_DOWN:
                case VK_UP:
                    
                    MainWindow::instance().pressed_non_fun_key = MainWindow::NON_FUNCTIONAL_KEY_NOT_PRESSED;
                    MainWindow::instance().pressed_fun_key = static_cast<WinKey>(wParam);

                    break;

                default:
                    
                    MainWindow::instance().pressed_non_fun_key = static_cast<int>(wParam);
                    MainWindow::instance().pressed_fun_key = WinKey::NOT_PRESSED;

                    break;
            }
        }

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

    HWND MainWindow::register_and_create_window(HINSTANCE hInstance, std::wstring const& window_name, bool resizable, int init_width, int init_height)
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
                (resizable ? WS_OVERLAPPEDWINDOW : (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)),
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

    WinKey MainWindow::get_pressed_functional_key() const noexcept
    {
        assert(pressed_non_fun_key == NON_FUNCTIONAL_KEY_NOT_PRESSED);

        return pressed_fun_key;
    }

    int MainWindow::get_pressed_non_functional_key() const noexcept
    {
        assert(pressed_fun_key == WinKey::NOT_PRESSED);

        return pressed_non_fun_key;
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
}

