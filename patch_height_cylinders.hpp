#if !defined(HEIGHT_PATCH_HPP)
#define HEIGHT_PATCH_HPP
#include "main.hpp"
namespace HEIGHT_PATCH
{
int calledTimes = 0;
kthook::kthook_simple<void*(__thiscall*)(void *this_)> generate_cylynder{0x7EDFE0, [&](const auto& hook, void* this_) {
        calledTimes++;
        return hook.get_trampoline()(this_);
    }
};

float maxHeight = 65; // 0xE4F868 -> 256.0
float minHeight = 60; // 0xE4F86C -> -256.0

struct HeightPatch
{
    HeightPatch()
    {
        DWORD oldProt;
        VirtualProtect(reinterpret_cast<void *>(0x7EE2B8 + 0x4), sizeof(float), PAGE_EXECUTE_READWRITE, &oldProt);
        // F30F 1015 6CF8E400                 movss   xmm2, ds: *pointer*
        *reinterpret_cast<float **>(0x7EE2B8 + 0x4) = &minHeight;
        VirtualProtect(reinterpret_cast<void *>(0x7EE2B8 + 0x4), sizeof(float), oldProt, &oldProt);
        VirtualProtect(reinterpret_cast<void *>(0x7EE336 + 0x4), sizeof(float), PAGE_EXECUTE_READWRITE, &oldProt);
        *reinterpret_cast<float **>(0x7EE336 + 0x4) = &maxHeight;
        VirtualProtect(reinterpret_cast<void *>(0x7EE336 + 0x4), sizeof(float), oldProt, &oldProt);

        ImGui::Hook::OnFrame += [](){
            ImGui::Begin("Height changer", 0, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Generate_Ring_Cylinders() was called %d times", calledTimes);
            ImGui::InputFloat("Max height(default 256.0)", &maxHeight);
            ImGui::InputFloat("Min height(default -256.0)", &minHeight);
            ImGui::Text("For apply need resize window");
            ImGui::End();
        };
    }
    ~HeightPatch() {}
} patch;
}
#endif