#pragma once
#include <d3d11.h>

namespace render {
	extern HWND hwnd;

	void presentScene(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);
	LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool& returnRes);
}