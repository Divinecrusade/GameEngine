#include <Windows.h>
#include <WinUser.h>
#include <cwchar>

static wchar_t szWindowClass[] = L"DesktopApp";
static wchar_t szTitle[] = L"Windows Desktop Application";

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    PAINTSTRUCT ps{ };
    HDC hdc{ };
    wchar_t const* greeting = L"Hello, Windows desktop!";

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
        szWindowClass,
        LoadIconW(hInstance, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&wcex))
    {
        MessageBoxW(NULL, L"Call to RegisterClassEx failed!", L"Windows Desktop", NULL);
        
        return EXIT_FAILURE;
    }

    HWND const hWnd 
    {
        CreateWindowExW(
            WS_EX_OVERLAPPEDWINDOW,
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            500, 100,
            NULL,
            NULL,
            hInstance,
            NULL
        )
    };
    if (!hWnd)
    {
        MessageBoxW(NULL, L"Call to CreateWindowEx failed!",
                          L"Windows Desktop",
                    NULL);

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

    return (int) msg.wParam;
}