#include "pch.h"
#include "Editor.h"

#include <fstream>
#include <ShObjIdl.h>

#include "Data/FileHelper.h"
#include "imgui/imgui.h"
#include "Windows/DX/UITexture.h"
#include "yaml-cpp/yaml.h"

Editor::Editor(const std::wstring& kName) :
    Actor(kName),
    file_path_(L""),
    loaded_texture_(nullptr)
{
}

void Editor::Tick(float delta_time)
{
    Actor::Tick(delta_time);
    
    const char* items[] = { "Reapet", "Clamp" };
    const char* filter_items[] = { "Point", "Bilinear" };
    
    static int current_wrap_mode = 0;
    static int current_filter_mode = 0;
    
    static float left_border = 0.f;
    static float right_border = 0.f;
    static float top_border = 0.f;
    static float bottom_border = 0.f;

    if (ImGui::Begin("Texture Editor"))
    {
        if (ImGui::Button("Load Texture"))
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
                            file_path_ = file_path;
                            
                            if (loaded_texture_) loaded_texture_.reset();
                            loaded_texture_ = std::make_unique<Texture>();

                            if (loaded_texture_->Load(file_path_))
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

        ImGui::SameLine();

        if (ImGui::Button("Save Meta Data"))
        {
            std::wofstream file(FileHelper::GetPath(file_path_) + L"\\" + FileHelper::GetBaseFilename(file_path_) + L".yaml");
            YAML::Emitter emitter;
            emitter << YAML::BeginMap;
                emitter << YAML::Key << "wrap_mode";
                emitter << YAML::Value << current_wrap_mode;
                emitter << YAML::Key << "filter_mode";
                emitter << YAML::Value << current_filter_mode;
                emitter << YAML::Key << "border";
                emitter << YAML::Value;
                emitter << YAML::BeginMap;
                    emitter << YAML::Key << "l";
                    emitter << YAML::Value << left_border;
                    emitter << YAML::Key << "r";
                    emitter << YAML::Value << right_border;
                    emitter << YAML::Key << "t";
                    emitter << YAML::Value << top_border;
                    emitter << YAML::Key << "b";
                    emitter << YAML::Value << bottom_border;
                emitter << YAML::EndMap;
            emitter << YAML::EndMap;

            file << emitter.c_str();
            file.close();
        }

        ImGui::Combo("Wrap Mode", &current_wrap_mode, items, IM_ARRAYSIZE(items));
        ImGui::Combo("Filter Mode", &current_filter_mode, filter_items, IM_ARRAYSIZE(filter_items));

        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("L", &left_border);
        
        ImGui::SameLine();
        
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("R", &right_border);
        
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("T", &top_border);
        
        ImGui::SameLine();
        
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("B", &bottom_border);

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
