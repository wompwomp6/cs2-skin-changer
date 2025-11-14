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

#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

namespace overlay {
    inline HWND Window = nullptr;
    inline WNDCLASSEXW wcex{};

    inline ID3D11Device* g_pd3dDevice = nullptr;
    inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    inline IDXGISwapChain* g_pSwapChain = nullptr;
    inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    inline ID3D11BlendState* g_pBlendState = nullptr;

    inline bool ShouldQuit = false;
    inline bool vsync = false;
    inline bool show_menu = true;

    inline int G_Width = GetSystemMetrics(SM_CXSCREEN);
    inline int G_Height = GetSystemMetrics(SM_CYSCREEN);

    inline int desired_fps = 120;

    void CreateRenderTarget() {
        ID3D11Texture2D* pBackBuffer = nullptr;
        if (SUCCEEDED(g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
            pBackBuffer->Release();
        }
    }

    void CreateBlendState() {
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.RenderTarget[0].BlendEnable = TRUE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        g_pd3dDevice->CreateBlendState(&blendDesc, &g_pBlendState);
    }

    bool CreateDeviceD3D(HWND hWnd) {
        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = G_Width;
        sd.BufferDesc.Height = G_Height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = vsync ? 60 : 0;
        sd.BufferDesc.RefreshRate.Denominator = vsync ? 1 : 0;
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
        CreateBlendState();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        return true;
    }

    void CleanupDeviceD3D() {
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
        if (g_pBlendState) { g_pBlendState->Release(); g_pBlendState = nullptr; }
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    }

    bool CreateShittyOverlay() {
        wcex = {
            sizeof(WNDCLASSEXW),
            0,
            DefWindowProcW,
            0,
            0,
            nullptr,
            LoadIcon(nullptr, IDI_APPLICATION),
            LoadCursor(nullptr, IDC_ARROW),
            nullptr,
            nullptr,
            L"Chrome_WidgetWin_1",
            LoadIcon(nullptr, IDI_APPLICATION)
        };

        RECT Rect;
        GetWindowRect(GetDesktopWindow(), &Rect);

        RegisterClassExW(&wcex);

        Window = CreateWindowExA(NULL, "Chrome_WidgetWin_1", "Discord Overlay", WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);

        SetWindowLong(Window, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
        MARGINS margin = { -1 };
        DwmExtendFrameIntoClientArea(Window, &margin);
        ShowWindow(Window, SW_SHOW);
        SetWindowPos(Window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 255, LWA_ALPHA);
        UpdateWindow(Window);
        return true;
    }

    bool hijack() {
        Window = FindWindowA("Chrome_WidgetWin_1", "Discord Overlay");
        if (!Window) {
            system("cls");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            MessageBoxA(0, "Failed To Find Discord.", "Overlay", MB_ICONSTOP);
            Sleep(5000);
            exit(1);
        }

        ShowWindow(Window, SW_SHOW);

        SetWindowLongA(Window, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
        SetWindowLongA(
            Window,
            -20,
            static_cast<LONG_PTR>(
                static_cast<int>(GetWindowLongA(Window, -20)) | 0x20
                )
        );

        MARGINS margin = { -1, -1, -1, -1 };
        DwmExtendFrameIntoClientArea(Window, &margin);

        SetLayeredWindowAttributes(Window, NULL, 0xFF, 0x02);

        SetWindowPos(Window, HWND_TOPMOST, 0, 0, 0, 0, 0x0002 | 0x0001);

        UpdateWindow(Window);
        return true;
    }

    bool Setup() {
        hijack();
        //MessageBoxA(0, "Press *OK* Once In Game", "Overlay", MB_ICONINFORMATION);
        return CreateDeviceD3D(Window);
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
        static auto next_frame_time = std::chrono::high_resolution_clock::now();

        const auto frame_interval = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
            std::chrono::duration<double>(1.0 / static_cast<double>(desired_fps))
        );

        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                ShouldQuit = true;
        }

        if (ShouldQuit) return;

        POINT p;
        GetCursorPos(&p);
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2((float)p.x, (float)p.y);
        io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        io.DeltaTime = 1.0f / 60.0f;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        auto now = std::chrono::high_resolution_clock::now();
        if (next_frame_time > now) {
            std::this_thread::sleep_until(next_frame_time);
        }
        next_frame_time += frame_interval;
    }

    void EndRender() {
        ImGui::Render();

        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

        float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
        g_pd3dDeviceContext->OMSetBlendState(g_pBlendState, blendFactor, 0xffffffff);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(vsync ? 1 : 0, 0);
    }
}