#pragma once
#include "pch.h"
#include "Overlay.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool menuShown = true;
bool pinMenu   = true;
HWND gameWindow;
WNDPROC gameWindowProc;
typedef BOOL(__stdcall* swapDong)(HDC);
swapDong oSwapBuffers;

LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Toggle the overlay using the delete key
    if (uMsg == WM_KEYDOWN && wParam == VK_DELETE) {
        menuShown = !menuShown;
        return false;
    }

    if (uMsg == WM_KEYDOWN && wParam == VK_END) {
        pinMenu = !pinMenu;
        return false;
    }

    // If the overlay is shown, direct input to the overlay only
    if (menuShown && !pinMenu) {
        CallWindowProc(ImGui_ImplWin32_WndProcHandler, hWnd, uMsg, wParam, lParam);
        return true;
    }

    // Otherwise call the game's wndProc function
    return CallWindowProc(gameWindowProc, hWnd, uMsg, wParam, lParam);
}

BOOL __stdcall hwglSwapBuffers(HDC hDc)
{
    // Initialize glew and imgui but only once
    static bool imGuiInitialized = false;
    if (!imGuiInitialized) {
        imGuiInitialized = true;

        // Get the game's window from it's handle
        gameWindow = WindowFromDC(hDc);

        // Overwrite the game's wndProc function
        gameWindowProc = (WNDPROC)SetWindowLongPtr(gameWindow,
            GWLP_WNDPROC, (LONG_PTR)windowProc_hook);

        // Init glew, create imgui context, init imgui
        glewInit();
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(gameWindow);
        ImGui_ImplOpenGL3_Init();

        NGetOverlay()->InitStyle();

        ImGui::CaptureMouseFromApp();
    }

    // If the menu is shown, start a new frame and draw the demo window
    if (menuShown) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        NGetOverlay()->StatTrackWindow();

        ImGui::Render();

        // Draw the overlay
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

	return oSwapBuffers(hDc);
}
