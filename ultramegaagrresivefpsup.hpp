//sorry for bad code, i am lazy...
#if !defined(ULTRAMEGAAGGRESIVEFPSUP_HPP)
#define ULTRAMEGAAGGRESIVEFPSUP_HPP
#include "main.hpp"

namespace ultraMegaAggressiveFPSUp
{
kthook::kthook_simple<HRESULT(__stdcall *)(IDirect3DDevice9 *, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT)> drawindexedprimitive_hook{ImGui::Hook::D3D9::GetVirtualTable()[82]};

HRESULT drawindexedprimitive_hooked(const decltype(drawindexedprimitive_hook) &hook, IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);

struct Stride
{
    bool render{false};
    int CurrentStride{0};
    INT LastWrittenStrade, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, StartIndex, PrimCount;
    INT myBaseVertexIndex, myMinVertexIndex, myNumVertices, myStartIndex, myPrimCount;
    bool applyMySettings{false};
    Stride()
    {
        ImGui::Hook::OnInitialize += [this]()
        {
            drawindexedprimitive_hook.set_cb(&drawindexedprimitive_hooked);
            drawindexedprimitive_hook.install();
            std::cout << "Show/hide VERY AGGRESIVE FPS UP xD - VK_RIGHT" << std::endl;
        };
        ImGui::Hook::OnFrame += [this]()
        {
            if(render && ImGui::Begin("VERY AGGRESIVE FPS UP xD", &render)) {
                ImGui::Text("Use key + or - or input");
                ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
                ImGui::InputInt("Stride", &CurrentStride);
                ImGui::Text("Stride info: ");
                ImGui::Text("Last blocked stride #%d\nPrimitiveType: %d\nBaseVertexIndex: %d\nMinVertexIndex: %d\nNumVertices: %d\nStartIndex: %d\nPrimCount: %d", LastWrittenStrade, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, StartIndex, PrimCount);
                if (ImGui::CollapsingHeader("I wanna edit this shit"))
                {
                    ImGui::InputInt("myBaseVertexIndex", &myBaseVertexIndex);
                    ImGui::InputInt("myMinVertexIndex", &myMinVertexIndex);
                    ImGui::InputInt("myNumVertices", &myNumVertices);
                    ImGui::InputInt("myStartIndex", &myStartIndex);
                    ImGui::InputInt("myPrimCount", &myPrimCount);
                    if (ImGui::Button(!applyMySettings ? "Apply":"Cancel"))
                        applyMySettings = !applyMySettings;
                }
                ImGui::End();
            }
        };
        ImGui::Hook::OnWndProc += [this](auto, auto msg, auto wparam, auto)
        {
            if (msg == WM_KEYDOWN && !ImGui::GetIO().WantCaptureKeyboard)
            {
                if (wparam == VK_RIGHT)
                {
                    render = !render;
                }
                else if (wparam == VK_OEM_PLUS)
                {
                    CurrentStride++;
                }
                else if (wparam == VK_OEM_MINUS)
                {
                    CurrentStride--;
                }
            }
            return 0;
        };
    }
    ~Stride() {}
} veryaggresivefpsup;

HRESULT drawindexedprimitive_hooked(const decltype(drawindexedprimitive_hook) &hook, IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
    LPDIRECT3DVERTEXBUFFER9 stream_data;
    UINT offset;
    UINT stride;
    
    if (SUCCEEDED(pDevice->GetStreamSource(0, &stream_data, &offset, &stride))) {
        if (stride == veryaggresivefpsup.CurrentStride)
        {
            if (veryaggresivefpsup.applyMySettings)
            {
                BaseVertexIndex = veryaggresivefpsup.myBaseVertexIndex;
                MinVertexIndex = veryaggresivefpsup.myMinVertexIndex;
                NumVertices = veryaggresivefpsup.myNumVertices;
                startIndex = veryaggresivefpsup.myStartIndex;
                primCount = veryaggresivefpsup.myPrimCount;
            }
            veryaggresivefpsup.PrimitiveType = PrimitiveType;
            veryaggresivefpsup.BaseVertexIndex = BaseVertexIndex;
            veryaggresivefpsup.MinVertexIndex = MinVertexIndex;
            veryaggresivefpsup.NumVertices = NumVertices;
            veryaggresivefpsup.StartIndex = startIndex;
            veryaggresivefpsup.PrimCount = primCount;
            if (veryaggresivefpsup.applyMySettings)
                hook.get_trampoline()(pDevice, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
            
            return 0;
        }
        stream_data->Release();
    }
    return hook.get_trampoline()(pDevice, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

}
#endif