// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cstdio>
#include <iostream>
#include <random>
#include "MinHook.h"
#include "RenderHook.h"
#include "NApplication.h"


void UnhookJamflexMouse()
{
    HHOOK jagexMouseHook = *(HHOOK*)((uintptr_t)GetModuleHandleA("rs2client.exe") + 0xBDF420);

    std::cout << "Mouse hook pointer: " << std::hex << jagexMouseHook << std::endl;
    if (UnhookWindowsHookEx(jagexMouseHook) != 0)
        std::cout << "Unhooked." << std::endl;
    std::dec;
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

uintptr_t Main(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    UnhookJamflexMouse();

    NApplication nApp;

    SetConsoleTitleA("NXTLite Development Console");
    std::cout << "rs2client.exe starts at\n\t" << std::hex << NGetApplication()->clientBase << std::endl;
    std::cout << "opengl32.dll  starts at\n\t" << std::hex << NGetApplication()->openglBase << std::endl;
    std::dec;

    MH_Initialize();

	MH_CreateHookApi(L"opengl32.dll", "wglSwapBuffers", &hwglSwapBuffers, reinterpret_cast<void**>(&oSwapBuffers));

    MH_EnableHook(MH_ALL_HOOKS);


    for (;;)
    {
        // Eject
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            //ImGui_ImplOpenGL3_Shutdown();
            //ImGui::DestroyContext();
            break;
        }
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
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