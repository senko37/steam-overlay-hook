#include <Windows.h>
#include "gameoverlay.hpp"

void init() {
    steam::init();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        init();

    return TRUE;
}