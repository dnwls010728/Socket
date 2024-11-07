#include "pch.h"
#include "Editor.h"

#include <complex.h>
#include <fstream>
#include <ShObjIdl.h>

#include "Data/FileHelper.h"
#include "imgui/imgui.h"
#include "Windows/DX/UITexture.h"
#include "yaml-cpp/yaml.h"

Editor::Editor(const std::wstring& kName) :
    Actor(kName),
    show_texture_settings_(false),
    wrap_mode_(0),
    filter_mode_(0),
    file_path_(L""),
    loaded_texture_(nullptr),
    frames_()
{
}

void Editor::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    ImGui::DockSpaceOverViewport(0);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Texture Settings", nullptr, &show_texture_settings_);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (show_texture_settings_) OpenTextureSettings(&show_texture_settings_);
}

void Editor::OpenTextureSettings(bool* is_open)
{
    if (!ImGui::Begin("Texture Settings", is_open))
    {
        ImGui::End();
        return;
    }

    static const char* wrap_modes[] = {"Repeat", "Clamp"};
    static const char* filter_modes[] = {"Point", "Bilinear"};

    if (ImGui::Button("Open Texture"))
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

                        if (!loaded_texture_->Load(file_path_))
                        {
                            loaded_texture_.reset();
                            file_path_ = L"";
                        }

                        CoTaskMemFree(file_path);
                    }

                    item->Release();
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Save Metadata"))
    {
        if (!loaded_texture_) return;

        std::wofstream file(file_path_ + L".yaml");
        YAML::Emitter emitter;

        emitter << YAML::BeginMap;
            emitter << YAML::Key << "wrap_mode";
            emitter << YAML::Value << 0;
            emitter << YAML::Key << "filter_mode";
            emitter << YAML::Value << 0;
            emitter << YAML::Key << "ppu";
            emitter << YAML::Value << 256;
            emitter << YAML::Key << "frames";
            emitter << YAML::Value << YAML::BeginSeq;

            for (int i = 0; i < 5; ++i)
            {
                emitter << YAML::BeginMap;
                    emitter << YAML::Key << "rect";
                    emitter << YAML::Value;
                    emitter << YAML::BeginMap;
                        emitter << YAML::Key << "x";
                        emitter << YAML::Value << 0;
                        emitter << YAML::Key << "y";
                        emitter << YAML::Value << 0;
                        emitter << YAML::Key << "width";
                        emitter << YAML::Value << 0;
                        emitter << YAML::Key << "height";
                        emitter << YAML::Value << 0;
                    emitter << YAML::EndMap;
                    emitter << YAML::Key << "pivot";
                    emitter << YAML::Value;
                    emitter << YAML::BeginMap;
                        emitter << YAML::Key << "x";
                        emitter << YAML::Value << 0;
                        emitter << YAML::Key << "y";
                        emitter << YAML::Value << 0;
                    emitter << YAML::EndMap;
                emitter << YAML::EndMap;
            }

            emitter << YAML::EndSeq;
        emitter << YAML::EndMap;

        file << emitter.c_str();
        file.close();
    }

    ImGui::Separator();

    ImGui::Combo("Wrap Mode", &wrap_mode_, wrap_modes, IM_ARRAYSIZE(wrap_modes));
    ImGui::Combo("Filter Mode", &filter_mode_, filter_modes, IM_ARRAYSIZE(filter_modes));

    if (ImGui::Button("Texture Editor"))
    {
    }
    
    ImGui::Separator();

    ImGui::Text("Preview");

    if (ImGui::BeginChild("Preview", {200.f, 200.f}, true, ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (loaded_texture_)
        {
            ImVec2 image_position = ImGui::GetCursorScreenPos();
            ImGui::Image(loaded_texture_->resource_view_.Get(), {loaded_texture_->GetWidth() * 1.f, loaded_texture_->GetHeight() * 1.f}, {0, 1}, {1, 0}, {1, 1, 1, 1}, {1, 1, 1, 1});
        }
    }

    ImGui::EndChild();

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
