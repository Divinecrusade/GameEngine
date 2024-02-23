#include <Windows.h>
#include <WinUser.h>
#include <cwchar>


static wchar_t const WINDOW_CLASS[] { L"DesktopApp" };
static wchar_t const WINDOW_TITLE[] { L"Rubber Thread" };

void show_error(wchar_t const* const msg)
{
    MessageBoxW(NULL, msg, WINDOW_TITLE, NULL);
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    PAINTSTRUCT ps{ };
    HDC hdc{ };
    wchar_t const* greeting{ L"Hello World!" };

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOutW(hdc, 5, 5, greeting, wcslen(greeting));

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

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    constexpr int WINDOW_WIDHT { 600 };
    constexpr int WINDOW_HEIGHT{ WINDOW_WIDHT };

    WNDCLASSEX const wcex
    {
        sizeof(wcex),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        LoadIconW(hInstance, IDI_APPLICATION),
        LoadCursorW(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        WINDOW_CLASS,
        LoadIconW(hInstance, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&wcex))
    {
        show_error(L"Call to RegisterClassEx failed!");

        return EXIT_FAILURE;
    }

    RECT window_pos{ };
    window_pos.left = 350;
    window_pos.right = WINDOW_WIDHT + window_pos.left;
    window_pos.top = 100;
    window_pos.bottom = WINDOW_HEIGHT + window_pos.top;
    DWORD const style{    static_cast<DWORD>(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) };
    DWORD const ex_style{ static_cast<DWORD>(WS_EX_OVERLAPPEDWINDOW) };
    AdjustWindowRectEx(&window_pos, style, FALSE, ex_style);
    HWND const hWnd
    {
        CreateWindowExW(
            ex_style,
            WINDOW_CLASS,
            WINDOW_TITLE,
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
        show_error(L"Call to CreateWindowEx failed!");

        return EXIT_FAILURE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg{ };
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}