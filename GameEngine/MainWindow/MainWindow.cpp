#include "MainWindow.hpp"
#include <stdexcept>
#include <cassert>


namespace GameEngine
{
    MainWindow* MainWindow::instance_{ nullptr };

    MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, std::optional<bool> resizable, std::optional<int> init_width, std::optional<int> init_height)
    :
    H_INSTANCE{ hInstance == NULL ? (HINSTANCE) GetModuleHandleW(NULL) : hInstance },
    WND_TITLE { window_name },
    H_WND     { MainWindow::register_and_create_window(H_INSTANCE, WND_TITLE, resizable.value_or(DEFAULT_RESIZABLE), init_width.value_or(DEFAULT_INIT_WIDTH), init_height.value_or(DEFAULT_INIT_HEIGHT)) }
    {
        assert(H_WND);
        assert(!instance_);

        instance_ = this;
        (void) ShowWindow(H_WND, nCmdShow);
        (void) UpdateWindow(H_WND);
    }

    MainWindow::~MainWindow() noexcept
    {
        if (!terminated)
        {
            try
            {
                if (!DestroyWindow(H_WND)) throw WinApiException{ "Failed to destroy window" };
            }
            catch (WinApiException const& e)
            {
                std::wstring const exception_msg{ e.get_full_description() };
#ifdef _DEBUG
                OutputDebugStringW(exception_msg.c_str());
#endif // _DEBUG
                MessageBoxW(NULL, exception_msg.c_str(), e.get_exception_class_name(), MB_OK);
            }
            terminated = true;
        }
        try
        {
            if (!UnregisterClassW(MainWindow::WND_CLASS_NAME, H_INSTANCE)) throw WinApiException{ "Failed to unregister window class" };
        }
        catch (WinApiException const& e)
        {
            std::wstring const exception_msg{ e.get_full_description() };
#ifdef _DEBUG
            OutputDebugStringW(exception_msg.c_str());
#endif // _DEBUG
            MessageBoxW(NULL, exception_msg.c_str(), e.get_exception_class_name(), MB_OK);
        }
        instance_ = nullptr;
    }

    LRESULT MainWindow::message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept
    {
        assert(hWnd);

        constexpr LRESULT MESSAGE_HANDLED{ 0 };

        switch (message)
        {
            case WM_LBUTTONDOWN:   MainWindow::instance_->key_pressed(VK_LBUTTON);  break;
            case WM_RBUTTONDOWN:   MainWindow::instance_->key_pressed(VK_RBUTTON);  break;
            case WM_MBUTTONDOWN:   MainWindow::instance_->key_pressed(VK_MBUTTON);  break;
            case WM_KEYDOWN:       MainWindow::instance_->key_pressed(wParam);      break;
            case WM_LBUTTONUP:     MainWindow::instance_->key_released(VK_LBUTTON); break;
            case WM_RBUTTONUP:     MainWindow::instance_->key_released(VK_RBUTTON); break;
            case WM_MBUTTONUP:     MainWindow::instance_->key_released(VK_MBUTTON); break;
            case WM_KEYUP:         MainWindow::instance_->key_released(wParam);     break;
            case WM_DISPLAYCHANGE: (void) InvalidateRect(hWnd, NULL, FALSE);        break;
            case WM_PAINT:         (void) ValidateRect(hWnd, NULL);                 break;
            case WM_DESTROY:       PostQuitMessage(EXIT_SUCCESS); MainWindow::instance_->terminated = true; break;
            default:               return DefWindowProcW(hWnd, message, wParam, lParam);
        }

        return MESSAGE_HANDLED;
    }

    HWND MainWindow::register_and_create_window(HINSTANCE hInstance, std::wstring_view window_name, bool resizable, int init_width, int init_height)
    {
        assert(hInstance);
        assert(init_width  > 0);
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
            if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
              throw WinApiException{ "Failed to register class" };

        RECT window_pos{ };
        window_pos.left = 350;
        window_pos.right = init_width + window_pos.left;
        window_pos.top = 100;
        window_pos.bottom = init_height + window_pos.top;
        DWORD const style{ static_cast<DWORD>((resizable ? WS_OVERLAPPEDWINDOW : (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX))) };
        DWORD const ex_style{ static_cast<DWORD>(WS_EX_OVERLAPPEDWINDOW) };
        AdjustWindowRectEx(&window_pos, style, FALSE, ex_style);
        HWND const hWnd
        {
            CreateWindowExW
            (
                ex_style,
                MainWindow::WND_CLASS_NAME,
                window_name.data(),
                style,
                window_pos.left, window_pos.top,
                window_pos.right - window_pos.left, window_pos.bottom - window_pos.top,
                NULL,
                NULL,
                hInstance,
                NULL
            )
        };
        if (!hWnd)
        {
            WinApiException e{ "Failed to create window" };

            (void) UnregisterClassW(MainWindow::WND_CLASS_NAME, hInstance);

            throw std::move(e);
        }

        return hWnd;
    }

    HWND MainWindow::get_window_handler() const noexcept
    {
        return H_WND;
    }

    bool MainWindow::is_fun_key_pressed(WinKey key) const noexcept
    {
        return std::ranges::find(pressed_fun_keys, key) != pressed_fun_keys.end();
    }

    bool MainWindow::is_non_fun_key_pressed(int code) const noexcept
    {
        return std::ranges::find(pressed_non_fun_keys, code) != pressed_non_fun_keys.end();
    }

    std::optional<WinKey> MainWindow::get_last_pressed_functional_key() const noexcept
    {
        return (!pressed_fun_keys.empty() ? std::optional<WinKey>{ pressed_fun_keys.back() } : std::nullopt);
    }

    std::optional<int> MainWindow::get_last_pressed_non_functional_key() const noexcept
    {
        return (!pressed_non_fun_keys.empty() ? std::optional<int>{ pressed_non_fun_keys.back() } : std::nullopt);
    }

    void MainWindow::key_pressed(WPARAM code) noexcept
    {
        switch (code)
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
            {
                if (WinKey const fun_key{ static_cast<WinKey>(code) }; !is_fun_key_pressed(fun_key))
                    pressed_fun_keys.push_back(fun_key);

                break;
            }
            default:
            {
                if (int const non_fun_key{ static_cast<int>(code) }; !is_non_fun_key_pressed(non_fun_key))
                    pressed_non_fun_keys.push_back(non_fun_key);

                break;
            }
        }
    }

    void MainWindow::key_released(WPARAM code) noexcept
    {
        switch (code)
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
            {
                pressed_fun_keys.erase(std::ranges::find(pressed_fun_keys, static_cast<WinKey>(code)));

                break;
            }
            default:
            {
                pressed_non_fun_keys.erase(std::ranges::find(pressed_non_fun_keys, static_cast<int>(code)));

                break;
            }
        }
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

