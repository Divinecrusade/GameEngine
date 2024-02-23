#include <Windows.h>
#include <cwchar>

static constexpr wchar_t WINDOW_CLASS[] { L"DesktopApp" };
static constexpr wchar_t WINDOW_TITLE[] { L"Rubber Thread" };

void show_error(wchar_t const* const msg)
{
    MessageBoxW(NULL, msg, WINDOW_TITLE, NULL);
}

__forceinline static POINT get_global_pos() noexcept
{
    POINT global_cursor_pos{ };

    GetCursorPos(&global_cursor_pos);

    return global_cursor_pos;
}

__forceinline POINT get_cursor_pos(HWND hWnd) noexcept
{
    POINT cursor_pos{ get_global_pos() };

    ScreenToClient(hWnd, &cursor_pos);

    return cursor_pos;
}

__forceinline void draw_line_by_primitives();

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    static PAINTSTRUCT ps{ };
    static HDC hdc{ };
    static bool LB_pressed{ false };
    static POINT line_beg{ };
    static POINT line_end{ };

    switch (message)
    {
        case WM_LBUTTONDOWN:
        {
            LB_pressed = true; 
            line_beg = get_cursor_pos(hWnd);
        }

        break;

        case WM_LBUTTONUP:
        {
            LB_pressed = false;
            line_beg.x = line_end.x = line_beg.y = line_end.y = 0;

            InvalidateRect(hWnd, nullptr, TRUE);
        }

        break;

        case WM_MOUSEMOVE: 
        {
            if (LB_pressed)
            { 
                line_end = get_cursor_pos(hWnd);
                InvalidateRect(hWnd, nullptr, TRUE);
            }
        }
        
        break;

        case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            
            HPEN const PEN{ CreatePen(PS_SOLID, 2, RGB(0, 0, 0)) };

            MoveToEx(hdc, line_beg.x, line_beg.y, nullptr);
            LineTo(hdc, line_end.x, line_end.y);

            EndPaint(hWnd, &ps);
        }

        break;

        case WM_DESTROY:
        {
            PostQuitMessage(EXIT_SUCCESS);
        }
  
        break;

        default: 
        {
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }

        break;
    }

    return EXIT_SUCCESS;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    constexpr int WINDOW_WIDHT { 600 };
    constexpr int WINDOW_HEIGHT{ WINDOW_WIDHT };
    constexpr int WINDOW_INIT_POS_X{ 300 };
    constexpr int WINDOW_INIT_POS_Y{ 100 };

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
    window_pos.left   = WINDOW_INIT_POS_X;
    window_pos.right  = WINDOW_WIDHT + window_pos.left;
    window_pos.top    = WINDOW_INIT_POS_Y;
    window_pos.bottom = WINDOW_HEIGHT + window_pos.top;
    DWORD const style   { static_cast<DWORD>(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) };
    DWORD const ex_style{ static_cast<DWORD>(WS_EX_OVERLAPPEDWINDOW) };
    AdjustWindowRectEx(&window_pos, style, FALSE, ex_style);
    HWND const hWnd
    {
        CreateWindowExW
        (
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