#if !defined(POPIT_GAME_HPP)
#define POPIT_GAME_HPP
#include "main.hpp"

class PopitGame
{
private:
    float radius;
    int amount;
    bool **matrix;
    unsigned int colors[7]{0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF00FFFF, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF};

public:
    bool render{false};
    PopitGame(float r, int amount_) : radius(r), amount(amount_)
    {
        assert((sizeof(colors) / sizeof(unsigned int)) >= amount);
        matrix = new bool *[amount];
        for (auto i = 0; i < amount; i++)
        {
            matrix[i] = new bool[amount];
            for (auto j = 0; j < amount; j++)
                matrix[i][j] = true;
        }
    };
    ~PopitGame()
    {
        for (auto i = 0; i < amount; i++)
            delete[] matrix[i];
        delete[] matrix;
    };
    void Draw()
    {
        ImGui::SetNextWindowSize(ImVec2((radius * 2) * amount + 5 * amount + 5, (radius * 2) * amount + 5 * amount + 25));
        ImGui::Begin("Popit", &render, ImGuiWindowFlags_NoResize);
        auto drawlist = ImGui::GetWindowDrawList();
        ImGui::SetCursorPos(ImVec2(radius + 5, radius + 25));
        auto pos = ImGui::GetCursorScreenPos();
        for (auto i = 0; i < amount; i++)
        {
            for (auto j = 0; j < amount; j++)
            {
                auto color = matrix[i][j] ? colors[i] : (colors[i] ^ ~0x50FFFFFF);
                drawlist->AddCircleFilled(pos, radius, color, 32);
                if (ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(ImVec2(pos.x - radius, pos.y - radius), ImVec2(pos.x + radius, pos.y + radius)))
                {
                    matrix[i][j] = !matrix[i][j];
                }
                pos = ImVec2(pos.x + (radius * 2) + 5, pos.y);
            }
            pos = ImVec2(pos.x - (radius * 2) * amount - 5 * amount, pos.y + (radius * 2) + 5);
        }
        ImGui::End();
    }
};
#endif