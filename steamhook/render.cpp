#include "render.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

HWND render::hwnd;
int width, height;

ID3D11Device* device;
ID3D11DeviceContext* context;
ID3D11RenderTargetView* targetView;

void render::presentScene(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
    if (!device) {
        ID3D11Texture2D* renderTarget = nullptr;
        ID3D11Texture2D* backBuffer = nullptr;
        D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };

        swapChain->GetDevice(__uuidof(device), reinterpret_cast<void**>(&device));
        device->GetImmediateContext(&context);

        swapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<void**>(&renderTarget));
        device->CreateRenderTargetView(renderTarget, nullptr, &targetView);
        renderTarget->Release();

        swapChain->GetBuffer(0, __uuidof(backBuffer), reinterpret_cast<void**>(&backBuffer));
        backBuffer->GetDesc(&backBufferDesc);
        width = backBufferDesc.Width;
        height = backBufferDesc.Height;
        backBuffer->Release();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        ImGui::StyleColorsLight();

        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(device, context);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize({ 800.f, 500.f });
    ImGui::Begin("Debug");

    ImGui::Text("Awesome!");

    ImGui::End();

    ImGui::Render();
    context->OMSetRenderTargets(1, &targetView, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT render::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, bool& returnRes) {
    if (hWnd != hwnd)
        return false;

    returnRes = false;
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);;
}