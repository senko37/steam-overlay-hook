#include "gameoverlay.hpp"
#include "utils.hpp"
#include "render.hpp"

typedef bool (*tSetupHook)(void* address, void* function, void* original, int a4);
typedef HRESULT(*tPresentDXGI)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);
typedef void(*tShowHideOverlay)(void* overlayInfo);

steam::sOverlayInfo* overlayInfo;

tPresentDXGI originalPresentDXGI;
HRESULT hookPresentDXGI(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
    render::presentScene(swapChain, syncInterval, flags);

    return originalPresentDXGI(swapChain, syncInterval, flags);
}

HHOOK wndProcHook;
WNDPROC originalWndProc;
LRESULT hookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    bool returnRes = false;
    LRESULT res = render::wndProc(hWnd, msg, wParam, lParam, returnRes);
    if (returnRes)
        return res;

    return originalWndProc(hWnd, msg, wParam, lParam);
}

bool steam::init() {
    UINT64 overlayBase = reinterpret_cast<UINT64>(GetModuleHandleA("GameOverlayRenderer64.dll"));
    if (!overlayBase)
        return false;

    tSetupHook setupHook = reinterpret_cast<tSetupHook>(utils::signatureScan(overlayBase, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x30\x33\xC0", "xxxx?xxxxxxx"));
    if (!setupHook)
        return false;

    UINT64 presentDXGILoc = utils::signatureScan(overlayBase, "\x48\x8B\x4F\x40\x48\x8D\x15\x00\x00\x00\x00\xE8", "xxxxxxx????x");
    if (!presentDXGILoc)
        return false;
    void* presentDXGI = reinterpret_cast<void*>(presentDXGILoc + *reinterpret_cast<INT32*>(presentDXGILoc + 7) + 11);

    UINT64 overlayInfoLoc = utils::signatureScan(overlayBase, "\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x45\x8B\xCF", "xxx????x????xxx");
    if (!overlayInfoLoc)
        return false;
    overlayInfo = reinterpret_cast<sOverlayInfo*>(overlayInfoLoc + *reinterpret_cast<INT32*>(overlayInfoLoc + 3) + 7);
   
    render::hwnd = FindWindowA("SDL_app", "Counter-Strike 2");
    if (!render::hwnd)
        render::hwnd = GetForegroundWindow();

    void* wndProc = reinterpret_cast<void*>(GetWindowLongPtrW(render::hwnd, GWLP_WNDPROC));
    if (!wndProc)
        return false;

    setupHook(presentDXGI, hookPresentDXGI, &originalPresentDXGI, 1);
    setupHook(wndProc, hookWndProc, &originalWndProc, 1);

    return true;
}