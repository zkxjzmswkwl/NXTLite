// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cstdio>
#include <iostream>
#include "MinHook.h"
#include <Windows.h>
#include <vector>
#include <random>
#include <gl/glew.h>
#include <gl/GL.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


uintptr_t clientBase = (uintptr_t)GetModuleHandleA("rs2client.exe");
uintptr_t openglBase = (uintptr_t)GetModuleHandleA("opengl32.dll");
std::string TITLE_TEXT = "NXTLite - github.com/zkxjzmswkwl/NXTLite";

void UnhookJamflexMouse()
{
    HHOOK jagexMouseHook = *(HHOOK*)(clientBase + 0xBDF420);

    std::cout << "Mouse hook pointer: " << std::hex << jagexMouseHook << std::endl;
    if (UnhookWindowsHookEx(jagexMouseHook) != 0)
        std::cout << "Unhooked." << std::endl;
}


typedef BOOL(__stdcall* iwglAids)(HDC);
iwglAids owglSwapBuffer = nullptr;
HGLRC WINAPI gameContext = nullptr;
HGLRC WINAPI ourContext = nullptr;


float randFloat()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(e);
}

struct Packet
{
    int64_t unk1;
    int64_t capacity;
    char* buffer;
    int64_t offset;
    int32_t unk2;
    int64_t unk3;
};

HWND hGameWindow;
WNDPROC hGameWindowProc;
bool menuShown = true;

LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Toggle the overlay using the delete key
    if (uMsg == WM_KEYDOWN && wParam == VK_DELETE) {
        menuShown = !menuShown;
        return false;
    }

    // If the overlay is shown, direct input to the overlay only
    if (menuShown) {
        CallWindowProc(ImGui_ImplWin32_WndProcHandler, hWnd, uMsg, wParam, lParam);
        return true;
    }

    // Otherwise call the game's wndProc function
    return CallWindowProc(hGameWindowProc, hWnd, uMsg, wParam, lParam);
}

BOOL __stdcall hwglSwapBuffers(HDC hDc)
{
    // Initialize glew and imgui but only once
    static bool imGuiInitialized = false;
    if (!imGuiInitialized) {
        imGuiInitialized = true;

        // Get the game's window from it's handle
        hGameWindow = WindowFromDC(hDc);

        // Overwrite the game's wndProc function
        hGameWindowProc = (WNDPROC)SetWindowLongPtr(hGameWindow,
            GWLP_WNDPROC, (LONG_PTR)windowProc_hook);

        // Init glew, create imgui context, init imgui
        glewInit();
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hGameWindow);
        ImGui_ImplOpenGL3_Init();

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(8.00f, 8.00f);
        style.FramePadding = ImVec2(5.00f, 2.00f);
        style.CellPadding = ImVec2(6.00f, 6.00f);
        style.ItemSpacing = ImVec2(6.00f, 6.00f);
        style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        style.IndentSpacing = 25;
        style.ScrollbarSize = 15;
        style.GrabMinSize = 10;
        style.WindowBorderSize = 1;
        style.ChildBorderSize = 1;
        style.PopupBorderSize = 1;
        style.FrameBorderSize = 1;
        style.TabBorderSize = 1;
        style.WindowRounding = 7;
        style.ChildRounding = 4;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ScrollbarRounding = 9;
        style.GrabRounding = 3;
        style.LogSliderDeadzone = 4;
        style.TabRounding = 4;

        ImGuiIO& io = ImGui::GetIO();
        //io.Fonts->AddFontFromFileTTF("C:\\Users\\Carter\\Desktop\\Ruda-Bold.ttf", 14);
        ImGui::CaptureMouseFromApp();
    }

    // If the menu is shown, start a new frame and draw the demo window
    if (menuShown) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        // Draw the overlay
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    //gameContext = wglGetCurrentContext();
    //if (ourContext == nullptr) {
    //    ourContext = wglCreateContext(hDC);
    //}


    //wglMakeCurrent(hDC, ourContext);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //GLint viewport[4];
    //glGetIntegerv(GL_VIEWPORT, viewport);
    ////std::cout << viewport[0] << "\t" << viewport[1] << "\t" << viewport[2] << "\t" << viewport[3] << std::endl;
    //glOrtho(0.0, viewport[2], viewport[3], 0.0, 1.0, -1.0);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //glClearColor(0, 0, 0, 1.0);



    // Trick from SO. When a RECT is 0x0x0x0 and a bitmap is written to it,
    // the rect structure is populated with exactly the dimensions needed to fit that bitmap.
    // We can use this as a surprisingly inexpensive way to retreieve the exact dimensions the text we'd like to render will take up.
    //HDC ha = GetDC(NULL);
    //RECT r = { 0, 0, 0, 0 };
    //DrawText(ha, TITLE_TEXT.c_str(), strlen(TITLE_TEXT.c_str()), &r, DT_CALCRECT);

    //int xCenter = (viewport[2] / 2) - (r.right / 2);

    //glColor3f(0, 0, 0);
    //glPrintBig(xCenter + 1, 15, TITLE_TEXT.c_str());
    //glColor3f(
    //    //randFloat(),
    //    //randFloat(),
    //    //randFloat(),
    //    1.0,
    //    0.2,
    //    0.2
    //);
    //glPrintBig(xCenter, 17, TITLE_TEXT.c_str());


	//wglMakeCurrent(hDC, gameContext);
	return owglSwapBuffer(hDc);
}
uintptr_t Main(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    UnhookJamflexMouse();

    SetConsoleTitleA("NXTLite Development Console");
    std::cout << "rs2client.exe starts at\n\t" << std::hex << clientBase << std::endl;
    std::cout << "opengl32.dll  starts at\n\t" << std::hex << openglBase << std::endl;

    MH_Initialize();

    MH_CreateHookApi(L"opengl32.dll", "wglSwapBuffers", &hwglSwapBuffers, reinterpret_cast<void**>(&owglSwapBuffer));

    MH_EnableHook(MH_ALL_HOOKS);

    for (;;)
    {
        // Deject
        if (GetAsyncKeyState(VK_F1) & 1)
            break;
    }

    MH_DisableHook(MH_ALL_HOOKS);
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(
            CreateThread(nullptr,
                0,
                (LPTHREAD_START_ROUTINE)Main,
                hModule,
                0,
                nullptr
            )
        );
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}