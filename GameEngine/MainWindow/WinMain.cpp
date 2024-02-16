#include "MainWindow.hpp"

#include <cassert>

#ifdef _DEBUG
#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif // _DEBUG


using namespace GameEngine;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow)
{
    int msg{ };
#ifdef _DEBUG
    {
#endif // _DEBUG
        MainWindow m_wnd{ hInstance, nCmdShow, L"Test App", true, 400, 500 };
        msg = (int)m_wnd.start_message_loop().wParam;
#ifdef _DEBUG
    }
#endif // _DEBUG

#ifdef _DEBUG
    assert(_CrtDumpMemoryLeaks() == FALSE);
#endif // _DEBUG

    return msg;
}