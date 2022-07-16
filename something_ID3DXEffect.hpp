#if !defined(SOMETHING_ID3DXEffect)
#define SOMETHING_ID3DXEffect
#include "main.hpp"
#include <d3dx9.h>

namespace d3dxEffect
{
    kthook::kthook_simple<HRESULT(__stdcall *)(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataLen, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors)> D3DXCreateEffect_hook{0xAC6602};

    std::vector<LPD3DXEFFECT> pEffects;
    struct ID3DXEffect
    {
        ID3DXEffect() {
            D3DXCreateEffect_hook.set_cb([](const auto& hook, auto pDevice, auto pSrcData, auto SrcDataLen, auto pDefines, auto pInclude, auto Flags, auto pPool, auto ppEffect, auto ppCopilationErrors) {
                std::cout << std::hex << "pSrcData " << pSrcData << " SrcDataLen " << SrcDataLen;
                auto res = hook.get_trampoline()(pDevice, pSrcData, SrcDataLen, pDefines, pInclude, Flags, pPool, ppEffect, ppCopilationErrors);
                LPD3DXEFFECT pEffect = *ppEffect;

                std::cout << " pEffect " << pEffect << std::endl;
                pEffects.push_back(pEffect);

                if (SUCCEEDED(pEffect->SetFloat("ScaleY", 1.0)))
                    std::cout << "SetFloat Succeeded" << std::endl;
                
                return res;
            });
            D3DXCreateEffect_hook.install();
        }
        ~ID3DXEffect() {}
    } effect;
}

#endif