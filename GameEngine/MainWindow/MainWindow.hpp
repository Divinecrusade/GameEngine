#pragma once

#include <IWindow.hpp>
#include <WinApiException.hpp>


namespace GameEngine
{
    class MainWindow final : public Interfaces::IWindow
    {
    public:

        static constexpr bool DEFAULT_RESIZABLE  { false };
        static constexpr int  DEFAULT_INIT_WIDTH {  640  };
        static constexpr int  DEFAULT_INIT_HEIGHT{  480  };

    public:

        MainWindow() = delete;
        MainWindow(HINSTANCE hInstance, int nCmdShow, std::wstring_view window_name, std::optional<bool> resizable = std::nullopt, std::optional<int> init_width = std::nullopt, std::optional<int> init_height = std::nullopt);
        MainWindow(MainWindow const&) = delete;
        MainWindow(MainWindow&&) = delete;

        virtual ~MainWindow() noexcept;

        MainWindow& operator=(MainWindow const&) = delete;
        MainWindow& operator=(MainWindow&&) = delete;

        bool is_fun_key_pressed(WinKey key) const noexcept override;
        bool is_non_fun_key_pressed(int code) const noexcept override;

        std::optional<WinKey> get_last_pressed_functional_key() const noexcept override;
        std::optional<int> get_last_pressed_non_functional_key() const noexcept override;

        void process_messages_queue() noexcept override;
        bool is_terminated() const noexcept override;
        HWND get_window_handler() const noexcept override;

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

        static LRESULT CALLBACK message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept;
        static HWND register_and_create_window(HINSTANCE hInstance, std::wstring_view window_name, bool resizable, int init_width, int init_height);

        void key_pressed(WPARAM code) noexcept;
        void key_released(WPARAM code) noexcept;

    private:

        static constexpr std::wstring_view WND_CLASS_NAME{ L"Main App Window" };

        static MainWindow* instance_;
    
        bool terminated{ false };

        HINSTANCE const H_INSTANCE;
        std::wstring const WND_TITLE;
        HWND const H_WND;

        std::vector<WinKey> pressed_fun_keys{ };
        std::vector<int> pressed_non_fun_keys{ };
    };
}
