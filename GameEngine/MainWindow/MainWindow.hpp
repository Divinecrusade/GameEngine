#pragma once

#include "IWindow.hpp"
#include <string>


class MainWindow : public IWindow
{
public:

    MainWindow() = delete;
    MainWindow(MainWindow const&) = delete;
    MainWindow(MainWindow&&) = delete;

    MainWindow& operator=(MainWindow const&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    static MainWindow& instance(HINSTANCE hInstance = nullptr, int nCmdShow = 0, std::wstring_view window_name = L"", size_t init_width = 0U, size_t init_height = 0U);
    
    virtual void process_messages_queue() noexcept override;
    virtual bool is_terminated() const noexcept override;

    __forceinline MSG start_message_loop() noexcept
    {
        MSG msg{ };
        while (GetMessageW(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            terminated = msg.message == WM_QUIT;
        }

        return msg;
    };

private:

    MainWindow(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, size_t init_width, size_t init_height);
    virtual ~MainWindow();

    static LRESULT CALLBACK message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept;
    static HWND register_and_create_window(HINSTANCE hInstance, std::wstring const& window_name, size_t init_width, size_t init_height);

private:

    static constexpr wchar_t const* WND_CLASS_NAME{ L"Main App Window" };
    
    bool terminated{ false };

    HINSTANCE const H_INSTANCE;
    std::wstring const WND_TITLE;
    HWND const H_WND;
};