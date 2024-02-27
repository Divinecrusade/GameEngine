#include <Windows.h>
#include <cwchar>
#include <cassert>


static constexpr wchar_t WINDOW_CLASS[] { L"DesktopApp" };
static constexpr wchar_t WINDOW_TITLE[] { L"Rubber Thread" };
static constexpr int WINDOW_WIDTH{ 600 };
static constexpr int WINDOW_HEIGHT{ WINDOW_WIDTH };

static constexpr int BASE_X{ WINDOW_WIDTH / 2 };
static constexpr int BASE_Y{ WINDOW_HEIGHT / 2 };


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

__forceinline void draw_line_by_primitives(HDC const& hdc, int x1, int y1, int x2, int y2) noexcept
{
    HPEN const PEN{ CreatePen(PS_SOLID, 2, RGB(0, 0, 0)) };

    MoveToEx(hdc, x1, y1, nullptr);
    LineTo(hdc, x2, y2);
}

__forceinline int convert_device_x_to_logic_x(int x)
{
    return x - BASE_X;
}

__forceinline int convert_logic_x_to_device_x(int x)
{
    return x + BASE_X;
}

__forceinline int convert_device_y_to_logic_y(int y)
{
    return (y > BASE_Y ? -(y - BASE_Y) : BASE_Y - y);
}

__forceinline int convert_logic_y_to_device_y(int y)
{
    return BASE_Y - y;
}

auto get_line_function(int x1, int y1, int x2, int y2)
{
    return [l = x2 - x1, m = y2 - y1, x1, y1](int x) 
    { 
        if (l == 0 || m == 0)
        {
            return y1;
        }
        else
        {
            float const t{ static_cast<float>(x - x1) / l };

            return static_cast<int>(m * t) + y1;
        }
    };
}

__forceinline void draw_line_by_pixels(HDC const& hdc, int x1, int y1, int x2, int y2) noexcept
{
    static COLORREF const PEN_C{ RGB(0, 0, 0) };

    if (max(x1, x2) - min(x1, x2) <= max(y1, y2) - min(y1, y2))
    {
        auto const fun
        {
            get_line_function
            (
                convert_device_y_to_logic_y(y1),
                convert_device_x_to_logic_x(x1),
                convert_device_y_to_logic_y(y2),
                convert_device_x_to_logic_x(x2)
            )
        };

        for (int device_y{ max(y1, y2) }; device_y >= min(y1, y2); --device_y)
        {
            int const logic_y{ convert_device_y_to_logic_y(device_y) };
            assert(logic_y >= -BASE_Y);
            assert(logic_y <= BASE_Y);
            int const logic_x{ fun(logic_y) };
            assert(logic_x >= -BASE_X);
            assert(logic_x <= BASE_X);
            int const device_x{ convert_logic_x_to_device_x(logic_x) };
            assert(device_x >= 0);
            assert(device_x <= WINDOW_WIDTH);

            //COLORREF const CANVAS_C{ GetPixel(hdc, device_x, fun(x)) };
            SetPixel(hdc, device_x, device_y, PEN_C);
        }
    }
    else
    {
        auto const fun
        { 
            get_line_function
            (
                convert_device_x_to_logic_x(x1),
                convert_device_y_to_logic_y(y1),
                convert_device_x_to_logic_x(x2),
                convert_device_y_to_logic_y(y2)
            )
        };

        for (int device_x{ min(x1, x2) }; device_x <= max(x1, x2); ++device_x)
        {
            int const logic_x{ convert_device_x_to_logic_x(device_x) };
            assert(logic_x >= -BASE_X);
            assert(logic_x <= BASE_X);
            int const logic_y{ fun(logic_x) };
            assert(logic_y >= -BASE_Y);
            assert(logic_y <= BASE_Y);
            int const device_y{ convert_logic_y_to_device_y(logic_y) };
            assert(device_y >= 0);
            assert(device_y <= WINDOW_HEIGHT);

            //COLORREF const CANVAS_C{ GetPixel(hdc, device_x, fun(x)) };
            SetPixel(hdc, device_x, device_y, PEN_C);
        }
    }
}

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
            
            draw_line_by_pixels(hdc, line_beg.x, line_beg.y, line_end.x, line_end.y);
            
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
    window_pos.right  = WINDOW_WIDTH + window_pos.left;
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