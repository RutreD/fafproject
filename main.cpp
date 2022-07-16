#include "main.hpp"

class Plugin
{
    kthook::kthook_simple<LRESULT(__thiscall *)(void *, UINT, WPARAM, LPARAM)> scfa_wndproc_hook{0x96D110};
    
    ImGui::Hook::D3D9 d3d9hook{};

    bool demo{false};
    bool menu{false};

    PopitGame popit{30 /*radius*/, 7 /*amount*/};

public:
    Plugin()
    {
        CreateConsole();
        ImGui::Hook::OnInitialize += [this]()
        {
            // install wndproc hook
            scfa_wndproc_hook.set_cb([](const decltype(scfa_wndproc_hook) &hook, auto this_, auto uMsg, auto wParam, auto lParam) -> LPARAM
                                     {
                if(ImGui::Hook::Input::WndProc(ImGui::Hook::Input::Window, uMsg, wParam, lParam))
                    return 1;
                return hook.get_trampoline()(this_, uMsg, wParam, lParam); });
            scfa_wndproc_hook.install();
            
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

            // add custom font and cyrillic characters
            ImVector<ImWchar> ranges;
            ImFontGlyphRangesBuilder builder;
            auto &io = ImGui::GetIO();

            std::string fontPath(MAX_PATH, '\0');
            if (!SHGetSpecialFolderPath(NULL, &fontPath[0], CSIDL_FONTS, FALSE))
                throw;
            fontPath.resize(std::strlen(fontPath.c_str()));
            fontPath += "\\trebucbd.ttf";

            builder.AddText("вЂљвЂћвЂ¦вЂ вЂЎв‚¬вЂ°вЂ№вЂ�вЂ™вЂњвЂќвЂўвЂ“вЂ”в„ўвЂєв„–");
            builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
            builder.BuildRanges(&ranges);
            io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 14.0, NULL, ranges.Data);
            io.Fonts->Build();
        };

        ImGui::Hook::OnFrame += [this]()
        {
            if (menu && ImGui::Begin("Test", &menu))
            {
                ImGui::Text("Hello world!");
                ImGui::Text("Привіт світ!");
                ImGui::Text("Привет мир!");
                if(ImGui::Button("Switch light style"))
                    ImGui::StyleColorsLight();
                ImGui::SameLine();
                if (ImGui::Button("Switch dark style"))
                    ImGui::StyleColorsDark();
                if (ImGui::Button("Switch classic style", ImVec2(218, 0)))
                    ImGui::StyleColorsClassic();
                ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
                ImGui::End();
            }
            if (demo)
                ImGui::ShowDemoWindow(&demo);
            if (popit.render)
                popit.Draw();
        };

        std::cout << "Show/hide demo - VK_UP" << std::endl;
        std::cout << "Show/hide test window - VK_LEFT" << std::endl;
        std::cout << "Show/hide popit game - VK_DOWN" << std::endl;
        ImGui::Hook::OnWndProc += [this](auto hwnd, auto msg, auto wparam, auto lparam)
        {
            if (msg == WM_KEYDOWN)
            {
                if (wparam == VK_UP)
                    demo = !demo;
                else if (wparam == VK_LEFT)
                    menu = !menu;
                else if (wparam == VK_DOWN)
                    popit.render = !popit.render;
            }
            return 0;
        };

        d3d9hook.Install(ImGui::Hook::D3D9::Method::EndScene); //Present - obs bypass, endscene - no
    };
    ~Plugin(){};
} plugin;
