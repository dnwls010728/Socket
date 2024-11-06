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
    loaded_texture_(nullptr),
    frames_()
{
}

void Editor::Tick(float delta_time)
{
    Actor::Tick(delta_time);
    
    const char* wrap_items[] = { "Reapet", "Clamp" };
    const char* filter_items[] = { "Point", "Bilinear" };
    
    static int current_wrap_mode = 0;
    static int current_filter_mode = 0;
    static int ppu = 100;

    ImGui::DockSpaceOverViewport(0);
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::EndMainMenuBar();
    }

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
            std::wofstream file(file_path_ + L".yaml");
            YAML::Emitter emitter;
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "wrap_mode";
            emitter << YAML::Value << current_wrap_mode;
            emitter << YAML::Key << "filter_mode";
            emitter << YAML::Value << current_filter_mode;
            emitter << YAML::Key << "ppu";
            emitter << YAML::Value << ppu;
            emitter << YAML::Key << "frames";
            emitter << YAML::Value;
            emitter << YAML::EndMap;

            file << emitter.c_str();
            file.close();
        }

        ImGui::Combo("Wrap Mode", &current_wrap_mode, wrap_items, IM_ARRAYSIZE(wrap_items));
        ImGui::Combo("Filter Mode", &current_filter_mode, filter_items, IM_ARRAYSIZE(filter_items));

        ImGui::InputInt("PPU", &ppu);
    }
    
    ImGui::End();

    if (ImGui::Begin("Texture"))
    {
        if (loaded_texture_)
        {
            ImVec2 image_position = ImGui::GetCursorScreenPos();
            ImGui::Image(loaded_texture_->resource_view_.Get(), {static_cast<float>(loaded_texture_->GetWidth()), static_cast<float>(loaded_texture_->GetHeight())}, {0.f, 0.f}, {1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, {0.f, 1.f, 0.f, 1.f});
            
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
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
