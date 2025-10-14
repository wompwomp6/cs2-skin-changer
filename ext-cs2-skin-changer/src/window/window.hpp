#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "../../ext/ImGui 1.90/imgui.h"
#include "../../ext/ImGui 1.90/imgui_impl_win32.h"
#include "../../ext/ImGui 1.90/imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace overlay {
    inline HWND Window = nullptr;
    inline WNDCLASSEXW wcex{};

    inline ID3D11Device* g_pd3dDevice = nullptr;
    inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    inline IDXGISwapChain* g_pSwapChain = nullptr;
    inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    inline bool ShouldQuit = false;
    inline bool vsync = false; // Keep vsync disabled for maximum performance

    inline bool clickThrough = true;
    inline bool previousClickThrough = true;

    inline int G_Width = GetSystemMetrics(SM_CXSCREEN);
    inline int G_Height = GetSystemMetrics(SM_CYSCREEN);

    inline int desired_fps = 300;

    void CreateRenderTarget() {
        ID3D11Texture2D* pBackBuffer = nullptr;
        if (SUCCEEDED(g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
            pBackBuffer->Release();
        }
    }

    bool CreateDeviceD3D(HWND hWnd) {
        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = G_Width;
        sd.BufferDesc.Height = G_Height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            featureLevels,
            _countof(featureLevels),
            D3D11_SDK_VERSION,
            &sd,
            &g_pSwapChain,
            &g_pd3dDevice,
            &featureLevel,
            &g_pd3dDeviceContext);

        if (FAILED(hr)) {
            std::cerr << "Failed to create D3D11 device and swap chain: 0x" << std::hex << hr << "\n";
            return false;
        }

        CreateRenderTarget();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        return true;
    }

    void CleanupDeviceD3D() {
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    }

    void UpdateWindowClickThrough() {
        LONG_PTR style = GetWindowLongPtr(Window, GWL_EXSTYLE);
        if (clickThrough) {
            style |= WS_EX_TRANSPARENT;
        }
        else {
            style &= ~WS_EX_TRANSPARENT;
        }
        SetWindowLongPtr(Window, GWL_EXSTYLE, style);
        SetWindowPos(Window, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    void SetupWindow(const std::wstring& windowName, const std::wstring& windowClass) {
        wcex = {
            sizeof(WNDCLASSEXW),
            CS_CLASSDC,
            WndProc,
            0, 0,
            GetModuleHandle(nullptr),
            LoadIcon(nullptr, IDI_APPLICATION),
            LoadCursor(nullptr, IDC_ARROW),
            nullptr, nullptr,
            windowClass.c_str(),
            nullptr
        };

        RegisterClassExW(&wcex);

        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);

        DWORD exStyle = WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW;
        if (clickThrough) {
            exStyle |= WS_EX_TRANSPARENT;
        }

        Window = CreateWindowExW(
            exStyle,
            windowClass.c_str(),
            windowName.c_str(),
            WS_POPUP,
            rect.left, rect.top,
            rect.right - rect.left, rect.bottom - rect.top,
            nullptr,
            nullptr,
            wcex.hInstance,
            nullptr
        );

        if (!Window) {
            std::cerr << "CreateWindowExW failed: " << GetLastError() << "\n";
            return;
        }

        SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 255, LWA_ALPHA);

        MARGINS margins = { -1 };
        DwmExtendFrameIntoClientArea(Window, &margins);

        ShowWindow(Window, SW_SHOW);
        UpdateWindow(Window);
    }

    void CloseOverlay() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
        DestroyWindow(Window);
        UnregisterClassW(wcex.lpszClassName, wcex.hInstance);
    }

    void Render() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                ShouldQuit = true;
        }

        if (ShouldQuit) return;

        if (clickThrough != previousClickThrough) {
            UpdateWindowClickThrough();
            previousClickThrough = clickThrough;
        }

        POINT p;
        GetCursorPos(&p);
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2((float)p.x, (float)p.y);
        io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        io.WantCaptureMouse = !clickThrough;
        io.WantCaptureKeyboard = !clickThrough;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Only apply frame rate limiting if desired_fps > 0
        if (desired_fps > 0) {
            static auto next_frame_time = std::chrono::high_resolution_clock::now();
            const auto frame_interval = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                std::chrono::duration<double>(1.0 / static_cast<double>(desired_fps))
            );

            auto now = std::chrono::high_resolution_clock::now();
            if (next_frame_time > now) {
                std::this_thread::sleep_until(next_frame_time);
            }
            next_frame_time += frame_interval;
        }
    }

    void EndRender() {
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(vsync ? 1 : 0, 0);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (!overlay::clickThrough) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}