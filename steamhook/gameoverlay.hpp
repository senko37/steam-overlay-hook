#pragma once
#include <Windows.h>

namespace steam {
    struct sOverlayInfo {
        char pad0[0x50];
        bool active;
        char pad1[0x1EF];
        HHOOK wndProc;
    };

    bool init();
}