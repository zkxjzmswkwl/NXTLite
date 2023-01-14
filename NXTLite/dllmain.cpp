// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cstdio>
#include <iostream>
#include "MinHook.h"
#include <Windows.h>
#include <vector>
#include <random>
#include <gl/GL.h>
#include "gltext.h"

uintptr_t clientBase = (uintptr_t)GetModuleHandleA("rs2client.exe");
uintptr_t openglBase = (uintptr_t)GetModuleHandleA("opengl32.dll");
std::string TITLE_TEXT = "NXTLite - github.com/zkxjzmswkwl/NXTLite";



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

BOOL __stdcall hwglSwapBuffers(HDC hDC)
{
    gameContext = wglGetCurrentContext();
    if (ourContext == nullptr) {
        ourContext = wglCreateContext(hDC);
    }


    wglMakeCurrent(hDC, ourContext);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    //std::cout << viewport[0] << "\t" << viewport[1] << "\t" << viewport[2] << "\t" << viewport[3] << std::endl;
    glOrtho(0.0, viewport[2], viewport[3], 0.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0, 0, 0, 1.0);


    // Trick from SO. When a RECT is 0x0x0x0 and a bitmap is written to it,
    // the rect structure is populated with exactly the dimensions needed to fit that bitmap.
    // We can use this as a surprisingly inexpensive way to retreieve the exact dimensions the text we'd like to render will take up.
    HDC ha = GetDC(NULL);
    RECT r = { 0, 0, 0, 0 };
    DrawText(ha, TITLE_TEXT.c_str(), strlen(TITLE_TEXT.c_str()), &r, DT_CALCRECT);

    int xCenter = (viewport[2] / 2) - (r.right / 2);

    glColor3f(0, 0, 0);
    glPrintBig(xCenter + 1, 15, TITLE_TEXT.c_str());
    glColor3f(
        //randFloat(),
        //randFloat(),
        //randFloat(),
        1.0,
        0.2,
        0.2
    );
    glPrintBig(xCenter, 17, TITLE_TEXT.c_str());


	wglMakeCurrent(hDC, gameContext);
	return owglSwapBuffer(hDC);
}

uintptr_t Main(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

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