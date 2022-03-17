// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

HMODULE hMod = NULL;

DWORD WINAPI KeyboardThread(LPVOID lpInfo);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hMod = hModule;
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, KeyboardThread, 0, 0, 0);
        MessageBox(0, L"Injection", L"Injection successful!", 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DWORD WINAPI KeyboardThread(LPVOID lpInfo) {
    while (true) {
        if (GetAsyncKeyState(VK_F2)) {
            break;
        }
        Sleep(100);
    }
    FreeLibraryAndExitThread(hMod, 0);
    return TRUE;
}