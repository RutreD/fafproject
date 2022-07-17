#if !defined(PATCH_HEIGHT_CYLINDERS)
#define PATCH_HEIGHT_CYLINDERS_HPP
#include "main.hpp"
#include <cmath>
namespace PATCH_HEIGHT_CYLINDERS
{
void *this_generates_cylinders;
float MAX_HEIGHT = 256.0;
float MIN_HEIGHT = -256.0; 

kthook::kthook_simple<void*(__thiscall*)(void *this_)> generate_cylinders_hook{0x7EDFE0, [&](const auto& hook, void* this_) {
        this_generates_cylinders = this_;
        return hook.get_trampoline()(this_);
    }
};

kthook::kthook_simple<void*(__thiscall*)(void *this_)> game_start_hook{0x7433B0, [&](const auto& hook, void* this_) {
        auto result = hook.get_trampoline()(this_);
        if(auto SIM = *reinterpret_cast<uintptr_t *>(0x10A63F0); SIM != NULL)
        {
            if(auto STIMap = *reinterpret_cast<uintptr_t *>(SIM + 0x8CC); STIMap != NULL) {
                auto mapSizeX = *reinterpret_cast<int *>(STIMap + 0x142C);
                auto mapSizeY = *reinterpret_cast<int *>(STIMap + 0x1430);
                if (auto MapData = *reinterpret_cast<uintptr_t *>(STIMap + 0x0); MapData != NULL && mapSizeX > 0 && mapSizeY > 0)
                {
                    if (auto TerrainHeights = *reinterpret_cast<uintptr_t *>(MapData + 0x0); TerrainHeights != NULL)
                    {

                        float maxCalculated = -256.0;
                        float minCalculated = 256.0;
                        for (auto x = 0; x < mapSizeX; x++)
                        {
                            for (auto y = 0; y < mapSizeY; y++)
                            {
                                float height = static_cast<float>((*reinterpret_cast<WORD *>(TerrainHeights + (y * mapSizeX + x) * 2))) * static_cast<float>(0.0078125);
                                if (height > maxCalculated)
                                    maxCalculated = height;
                                if (height < minCalculated)
                                    minCalculated = height;
                            }
                        }
                        MIN_HEIGHT = std::ceil(minCalculated - 1);
                        MAX_HEIGHT = std::ceil(maxCalculated + 1);
                        
                        DWORD oldProt;
                        VirtualProtect(reinterpret_cast<void *>(0x7EE2B8 + 0x4), sizeof(float), PAGE_EXECUTE_READWRITE, &oldProt);
                        // F30F 1015 6CF8E400                 movss   xmm2, ds: *pointer*
                        *reinterpret_cast<float **>(0x7EE2B8 + 0x4) = &MIN_HEIGHT;
                        VirtualProtect(reinterpret_cast<void *>(0x7EE2B8 + 0x4), sizeof(float), oldProt, &oldProt);
                        VirtualProtect(reinterpret_cast<void *>(0x7EE336 + 0x4), sizeof(float), PAGE_EXECUTE_READWRITE, &oldProt);
                        *reinterpret_cast<float **>(0x7EE336 + 0x4) = &MAX_HEIGHT;
                        VirtualProtect(reinterpret_cast<void *>(0x7EE336 + 0x4), sizeof(float), oldProt, &oldProt);

                        generate_cylinders_hook.get_trampoline()(this_generates_cylinders);
                    }
                }
            }
        }
        return result;
    }
};

}

#endif