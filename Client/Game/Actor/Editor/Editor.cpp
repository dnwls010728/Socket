#include "pch.h"
#include "Editor.h"

#include <ShObjIdl.h>

#include "imgui/imgui.h"
#include "Windows/DX/UITexture.h"

Editor::Editor(const std::wstring& kName) :
    Actor(kName),
    loaded_texture_(nullptr)
{
}

void Editor::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    if (ImGui::Begin("Editor"))
    {
        if (ImGui::Button("Load File"))
        {
            IFileOpenDialog* file_open;
            HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&file_open));
            if (SUCCEEDED(hr))
            {
                hr = file_open->Show(NULL);
                if (SUCCEEDED(hr))
                {
                    IShellItem* item;
                    hr = file_open->GetResult(&item);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR file_path;
                        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &file_path);
                        if (SUCCEEDED(hr))
                        {
                            if (loaded_texture_) loaded_texture_.reset();
                            loaded_texture_ = std::make_unique<Texture>();

                            if (loaded_texture_->Load(file_path))
                            {
                            }
                            
                            CoTaskMemFree(file_path);
                        }
                        
                        item->Release();
                    }
                }
                
                file_open->Release();
            }
        }

        static float left_border = 0.f;
        static float right_border = 0.f;
        static float top_border = 0.f;
        static float bottom_border = 0.f;

        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("Left Border", &left_border);
        
        ImGui::SameLine();
        
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("Right Border", &right_border);
        
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("Top Border", &top_border);
        
        ImGui::SameLine();
        
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("Bottom Border", &bottom_border);

        if (loaded_texture_)
        {
            ImVec2 image_position = ImGui::GetCursorScreenPos();
            ImGui::Image(loaded_texture_->resource_view_.Get(), {static_cast<float>(loaded_texture_->GetWidth()), static_cast<float>(loaded_texture_->GetHeight())}, {0.f, 0.f}, {1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 1.f, 0.f, 1.f});
            
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddLine({image_position.x + left_border, image_position.y}, {image_position.x + left_border, image_position.y + loaded_texture_->GetHeight()}, IM_COL32(255, 0, 0, 255));
            draw_list->AddLine({image_position.x + loaded_texture_->GetWidth() - right_border, image_position.y}, {image_position.x + loaded_texture_->GetWidth() - right_border, image_position.y + loaded_texture_->GetHeight()}, IM_COL32(255, 0, 0, 255));
            draw_list->AddLine({image_position.x, image_position.y + top_border}, {image_position.x + loaded_texture_->GetWidth(), image_position.y + top_border}, IM_COL32(255, 0, 0, 255));
            draw_list->AddLine({image_position.x, image_position.y + loaded_texture_->GetHeight() - bottom_border}, {image_position.x + loaded_texture_->GetWidth(), image_position.y + loaded_texture_->GetHeight() - bottom_border}, IM_COL32(255, 0, 0, 255));
        }
    }

    ImGui::End();
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Editor>("Editor")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
