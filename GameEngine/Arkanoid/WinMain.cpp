#include "Arkanoid.hpp"
#include <MainWindow/MainWindow.hpp>
#include <GraphicsDirect2D.hpp>

#ifdef _DEBUG
#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif // _DEBUG


using namespace GameEngine;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow)
{
#ifdef _DEBUG
    
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    
#endif // _DEBUG
#ifdef _DEBUG
    {
#endif // _DEBUG
        MainWindow m_wnd{ hInstance, nCmdShow, L"Arkanoid", Arkanoid::RESIZABLE, Arkanoid::WINDOW.get_width(), Arkanoid::WINDOW.get_height() };
        GraphicsDirect2D graphics{ m_wnd.get_window_handler() };
        Arkanoid theGame{ m_wnd, graphics };
        theGame.start();
#ifdef _DEBUG
    }
#endif // _DEBUG

#ifdef _DEBUG
    assert(_CrtDumpMemoryLeaks() == FALSE);
#endif // _DEBUG

    return EXIT_SUCCESS;
}