#if !defined(MAIN_HPP)
#define MAIN_HPP
#include <iostream>
#include <Windows.h>
#include <shlobj_core.h>
#include <thread>
#include <chrono>

#define USE_KTHOOKWNDPROC // in any game i can just ImGui::Hook::Input::Install(), but supreme commander doesnt call standart wndproc for mouse events, need install custom hook
#include "imgui_hook.hpp"


#include "ultramegaagrresivefpsup.hpp"
#include "popit_game.hpp"
#include "print_d3d9_vtable.hpp"
#include "patch_height_cylinders.hpp"

void CreateConsole()
{
    if (!AllocConsole())
        return;
    FILE *fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    HANDLE hConOut = CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
    SetStdHandle(STD_ERROR_HANDLE, hConOut);
    SetStdHandle(STD_INPUT_HANDLE, hConIn);
    std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();
    std::wcin.clear();
}

#endif