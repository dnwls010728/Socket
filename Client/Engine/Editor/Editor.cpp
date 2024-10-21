#include "pch.h"
#include "Editor.h"

#include "Logger.h"
#include "Actor/Actor.h"
#include "imgui/imgui.h"
#include "Level/World.h"
#include "Resource/ResourceManager.h"

#include "Windows/WindowsWindow.h"
#include "Windows/DX/Sprite.h"

Editor::Editor() :
    show_hierarchy_(false),
    show_animator_(false),
    show_sprite_editor_(false),
    sprite_(nullptr)
{
    // if (ResourceManager::Get()->Load<Sprite>(L"PlayerSheet", L".\\Game_Data\\PlayerSheet.png"))
    // {
    //     sprite_ = ResourceManager::Get()->GetResource<Sprite>(L"PlayerSheet");
    // }
}

void Editor::Tick(float delta_time)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                WindowsWindow* window = World::Get()->GetWindow();
                PostMessage(window->GetHWnd(), WM_USER, 0, 0);
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Hierarchy", nullptr, &show_hierarchy_);
            ImGui::MenuItem("Animator", nullptr, &show_animator_);
            ImGui::EndMenu();
        }
    }
    
    ImGui::EndMainMenuBar();

    if (show_hierarchy_) ShowHierarchy(&show_hierarchy_);
    if (show_animator_) ShowAnimator(&show_animator_);
    if (show_sprite_editor_) ShowSpriteEditor(&show_sprite_editor_);
}

void Editor::ShowHierarchy(bool* p_open)
{
    if (!ImGui::Begin("Hierarchy", p_open))
    {
        ImGui::End();
        return;
    }

    if (ImGui::TreeNode("Root"))
    {
        for (const auto& actor : World::Get()->current_level_->actors_)
        {
            std::string name;
            name.assign(actor->GetName().begin(), actor->GetName().end());

            if (actor->IsActive())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, {1.f, 1.f, 1.f, 1.f});
                ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                ImGui::PopStyleColor();
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, {.5f, .5f, .5f, 1.f});
                ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                ImGui::PopStyleColor();
            }
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void Editor::ShowAnimator(bool* p_open)
{
    if (!ImGui::Begin("Animator", p_open))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Open Sprite Editor"))
    {
        show_sprite_editor_ = !show_sprite_editor_;
    }

    if (sprite_)
    {
        ImGui::Image(sprite_->resource_view_.Get(), {300.f, 300.f}, {0.f, 0.f}, {1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f});
    }

    ImGui::End();
}

void Editor::ShowSpriteEditor(bool* p_open)
{
    if (!ImGui::Begin("Sprite Editor", p_open))
    {
        ImGui::End();
        return;
    }
    
    static ImVec2 selected_tile = { 0.f, 0.f };

    static int tile_size[2] = { 32, 32 };
    if (ImGui::InputInt2("Tile Size", tile_size))
    {
        if (tile_size[0] < 1) tile_size[0] = 1;
        if (tile_size[1] < 1) tile_size[1] = 1;
        
        selected_tile = { 0.f, 0.f };
    }

    const ImVec2 start_pos = ImGui::GetCursorScreenPos();

    if (sprite_)
    {
        ImGui::Image(sprite_->resource_view_.Get(), ImVec2(sprite_->width_, sprite_->height_));
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    const int tile_x_count = sprite_->width_ / tile_size[0];
    const int tile_y_count = sprite_->height_ / tile_size[1];

    for (int x = 0; x < tile_x_count + 1; ++x)
    {
        draw_list->AddLine({start_pos.x + x * tile_size[0], start_pos.y}, {start_pos.x + x * tile_size[0], start_pos.y + sprite_->height_}, IM_COL32(255, 255, 255, 255));
    }

    for (int y = 0; y < tile_y_count + 1; ++y)
    {
        draw_list->AddLine({start_pos.x, start_pos.y + y * tile_size[1]}, {start_pos.x + sprite_->width_, start_pos.y + y * tile_size[1]}, IM_COL32(255, 255, 255, 255));
    }

    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(0))
        {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            ImVec2 local_pos = { mouse_pos.x - start_pos.x, mouse_pos.y - start_pos.y };

            selected_tile.x = std::floor(local_pos.x / tile_size[0]) * tile_size[0];
            selected_tile.y = std::floor(local_pos.y / tile_size[1]) * tile_size[1];
        }
    }

    ImVec2 tl = { start_pos.x + selected_tile.x, start_pos.y + selected_tile.y };
    ImVec2 br = { start_pos.x + selected_tile.x + tile_size[0] + 1.f, start_pos.y + selected_tile.y + tile_size[1] + 1.f };
    draw_list->AddRect(tl, br, IM_COL32(255, 0, 0, 255));

    ImGui::End();
}
