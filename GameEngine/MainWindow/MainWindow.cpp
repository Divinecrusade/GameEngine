#include "MainWindow.hpp"


namespace GameEngine
{
    MainWindow* MainWindow::instance_{ nullptr };

    MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, std::optional<bool> resizable, std::optional<int> init_width, std::optional<int> init_height, std::optional<int> init_left_top_x_pos, std::optional<int> init_left_top_y_pos)
    :
    H_INSTANCE{ hInstance == NULL ? (HINSTANCE) GetModuleHandleW(NULL) : hInstance },
    WND_TITLE { window_name },
    H_WND     { MainWindow::register_and_create_window(H_INSTANCE, WND_TITLE, resizable.value_or(DEFAULT_RESIZABLE), init_width.value_or(DEFAULT_INIT_WIDTH), init_height.value_or(DEFAULT_INIT_HEIGHT), init_left_top_x_pos.value_or(DEFAULT_INIT_LEFT_TOP_X_POS), init_left_top_y_pos.value_or(DEFAULT_INIT_LEFT_TOP_Y_POS)) }
    {
        assert(H_WND);
        assert(!instance_);

        instance_ = this;
        std::ignore = ShowWindow(H_WND, nCmdShow);
        std::ignore = UpdateWindow(H_WND);
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
                OutputDebugStringW(L"\n");
#endif // _DEBUG
                MessageBoxW(NULL, exception_msg.c_str(), e.get_exception_class_name(), MB_OK);
            }
            terminated = true;
        }
        try
        {
            if (!UnregisterClassW(MainWindow::WND_CLASS_NAME.data(), H_INSTANCE)) throw WinApiException{"Failed to unregister window class"};
        }
        catch (WinApiException const& e)
        {
            std::wstring const exception_msg{ e.get_full_description() };
#ifdef _DEBUG
            OutputDebugStringW(exception_msg.c_str());
            OutputDebugStringW(L"\n");
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
            case WM_DISPLAYCHANGE: std::ignore = InvalidateRect(hWnd, NULL, FALSE); break;
            case WM_PAINT:         std::ignore = ValidateRect(hWnd, NULL);          break;
            case WM_DESTROY:       PostQuitMessage(EXIT_SUCCESS); MainWindow::instance_->terminated = true; break;
            case WM_MOUSEMOVE:     MainWindow::instance_->cur_mouse_pos = Geometry::Vector2D<int>{ LOWORD(lParam), HIWORD(lParam) }; break;
            case WM_MOUSEWHEEL:    MainWindow::instance_->mouse_wheel_distance_rotation = HIWORD(wParam) / WHEEL_DELTA; break;
            default:               return DefWindowProcW(hWnd, message, wParam, lParam);
        }

        return MESSAGE_HANDLED;
    }

    HWND MainWindow::register_and_create_window(HINSTANCE hInstance, std::wstring_view window_name, bool resizable, int init_width, int init_height, int init_left_top_x_pos, int init_left_top_y_pos)
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
            MainWindow::WND_CLASS_NAME.data(),
            LoadIconW(hInstance, IDI_APPLICATION)
        };

        if (!RegisterClassExW(&wcex))
            if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
              throw WinApiException{ "Failed to register class" };

        RECT window_pos{ };
        window_pos.left   = init_left_top_x_pos;
        window_pos.right  = init_width + window_pos.left;
        window_pos.top    = init_left_top_y_pos;
        window_pos.bottom = init_height + window_pos.top;
        DWORD const style{ static_cast<DWORD>((resizable ? WS_OVERLAPPEDWINDOW : (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX))) };
        constexpr DWORD ex_style{ static_cast<DWORD>(WS_EX_OVERLAPPEDWINDOW) };
        AdjustWindowRectEx(&window_pos, style, FALSE, ex_style);
        HWND const hWnd
        {
            CreateWindowExW
            (
                ex_style,
                MainWindow::WND_CLASS_NAME.data(),
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

            std::ignore = UnregisterClassW(MainWindow::WND_CLASS_NAME.data(), hInstance);

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
            case VK_DELETE:
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
            case VK_DELETE:
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

    Geometry::Vector2D<int> const& MainWindow::get_mouse_pos() const noexcept
    {
        return cur_mouse_pos;
    }

    int MainWindow::get_mouse_wheel_rotation_destance() const noexcept
    {
        return std::exchange(mouse_wheel_distance_rotation, 0);
    }
}

