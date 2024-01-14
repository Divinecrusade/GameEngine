#pragma once

#include "IWindow.hpp"
#include <string>


class MainWindow : public IWindow
{
public:

    MainWindow() = delete;
    MainWindow(HINSTANCE _In_ hInstance, _In_ int nCmdShow, std::wstring_view window_name, size_t init_width, size_t init_height);
    MainWindow(MainWindow const&) = delete;
    MainWindow(MainWindow&&) = delete;

    MainWindow& operator=(MainWindow const&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    virtual ~MainWindow();

private:

    static LRESULT CALLBACK message_handler(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

private:

    static constexpr wchar_t const* WND_CLASS_NAME{ L"Main App Window" };
    
    HINSTANCE const H_INSTANCE;
    std::wstring const WND_TITLE;
};