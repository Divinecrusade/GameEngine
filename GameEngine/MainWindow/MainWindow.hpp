#pragma once

#include "../IWindow.hpp"
#include <string>
#include <vector>
#include <algorithm>


namespace GameEngine
{
    class MainWindow final : public Interfaces::IWindow
    {
    public:

        static constexpr int DEFAULT_INIT_WIDTH{ 640 };
        static constexpr int DEFAULT_INIT_HEIGHT{ 480 };

        static constexpr int NON_FUNCTIONAL_KEY_NOT_PRESSED{ 0U };

    public:

        MainWindow() = delete;
        MainWindow(MainWindow const&) = delete;
        MainWindow(MainWindow&&) = delete;

        MainWindow& operator=(MainWindow const&) = delete;
        MainWindow& operator=(MainWindow&&) = delete;

        static MainWindow& instance(HINSTANCE hInstance = nullptr, int nCmdShow = 0, std::wstring_view window_name = L"", bool resizable = true, int init_width = DEFAULT_INIT_WIDTH, int init_height = DEFAULT_INIT_HEIGHT);
    
        virtual bool is_fun_key_pressed(WinKey key) const override;
        virtual bool is_non_fun_key_pressed(int code) const override;

        virtual WinKey get_last_pressed_functional_key() const noexcept override;
        virtual int get_last_pressed_non_functional_key() const noexcept override;

        virtual void process_messages_queue() noexcept override;
        virtual bool is_terminated() const noexcept override;
        virtual HWND get_window_handler() const noexcept override;

        __forceinline MSG start_message_loop() noexcept
        {
            MSG msg{ };
            while (GetMessageW(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

            return msg;
        };

    private:

        MainWindow(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, bool resizable, int init_width, int init_height);
        virtual ~MainWindow();

        static LRESULT CALLBACK message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept;
        static HWND register_and_create_window(HINSTANCE hInstance, std::wstring const& window_name, bool resizable, int init_width, int init_height);

        void key_pressed(WPARAM code);
        void key_released(WPARAM code);

    private:

        static constexpr wchar_t const* WND_CLASS_NAME{ L"Main App Window" };
    
        bool terminated{ false };

        HINSTANCE const H_INSTANCE;
        std::wstring const WND_TITLE;
        HWND const H_WND;

        std::vector<WinKey> pressed_fun_keys{ };
        std::vector<int> pressed_non_fun_keys{ };
    };
}
