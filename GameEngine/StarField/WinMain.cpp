#include "StarField.hpp"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow)
{
    StarField game{ hInstance, nCmdShow };
    game.start();

    return EXIT_SUCCESS;
}
