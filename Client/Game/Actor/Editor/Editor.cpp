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
    show_texture_settings_(true),
    show_texture_editor_(false),
    wrap_mode_(0),
    filter_mode_(0),
    ppu_(256),
    pivot_x_(.5f),
    pivot_y_(.5f),
    file_path_(L""),
    loaded_texture_(nullptr),
    frame_names_(),
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
    if (show_texture_editor_) OpenTextureEditor(&show_texture_editor_);
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
            emitter << YAML::Value << wrap_mode_;
            emitter << YAML::Key << "filter_mode";
            emitter << YAML::Value << filter_mode_;
            emitter << YAML::Key << "ppu";
            emitter << YAML::Value << ppu_;
            emitter << YAML::Key << "frames";
            emitter << YAML::Value << YAML::BeginSeq;

            for (const FrameData& frame : frames_)
            {
                emitter << YAML::BeginMap;
                    emitter << YAML::Key << "rect";
                    emitter << YAML::Value;
                    emitter << YAML::BeginMap;
                        emitter << YAML::Key << "x";
                        emitter << YAML::Value << frame.x;
                        emitter << YAML::Key << "y";
                        emitter << YAML::Value << frame.y;
                        emitter << YAML::Key << "width";
                        emitter << YAML::Value << frame.width;
                        emitter << YAML::Key << "height";
                        emitter << YAML::Value << frame.height;
                    emitter << YAML::EndMap;
                    emitter << YAML::Key << "pivot";
                    emitter << YAML::Value;
                    emitter << YAML::BeginMap;
                        emitter << YAML::Key << "x";
                        emitter << YAML::Value << frame.pivot_x;
                        emitter << YAML::Key << "y";
                        emitter << YAML::Value << frame.pivot_y;
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
    ImGui::InputInt("PPU", &ppu_);

    static int selected_frame_ = 0;

    ImGui::ListBox("Frames", &selected_frame_, frame_names_.data(), frame_names_.size());
    
    if (ImGui::Button("Texture Editor"))
    {
        show_texture_editor_ = true;
    }
    
    ImGui::Separator();
    ImGui::Text("Preview");
    if (ImGui::BeginChild("Preview", {200.f, 200.f}, true, ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (loaded_texture_ && !frames_.empty())
        {
            ImVec2 uv0 = {frames_[selected_frame_].x, frames_[selected_frame_].y};
            ImVec2 uv1 = {frames_[selected_frame_].x + frames_[selected_frame_].width, frames_[selected_frame_].y + frames_[selected_frame_].height};
            ImGui::Image(loaded_texture_->resource_view_.Get(), {loaded_texture_->GetWidth() * 1.f, loaded_texture_->GetHeight() * 1.f}, uv0, uv1, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f});
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

void Editor::OpenTextureEditor(bool* is_open)
{
    if (!ImGui::Begin("Texture Editor", is_open))
    {
        ImGui::End();
        return;
    }

    static int grid[2];

    static const char* pivot_modes[] = {"Center", "Top Left", "Top", "Top Right", "Left", "Right", "Bottom Left", "Bottom", "Bottom Right"};

    static int selected_pivot_mode = 0;

    ImGui::Text("Auto Slice");
    ImGui::InputInt2("Grid", grid);

    if (ImGui::Combo("Pivot", &selected_pivot_mode, pivot_modes, IM_ARRAYSIZE(pivot_modes)))
    {
        if (selected_pivot_mode == 0)
        {
            pivot_x_ = .5f;
            pivot_y_ = .5f;
        }
        else if (selected_pivot_mode == 1)
        {
            pivot_x_ = 0.f;
            pivot_y_ = 1.f;
        }
        else if (selected_pivot_mode == 2)
        {
            pivot_x_ = .5f;
            pivot_y_ = 1.f;
        }
        else if (selected_pivot_mode == 3)
        {
            pivot_x_ = 1.f;
            pivot_y_ = 1.f;
        }
        else if (selected_pivot_mode == 4)
        {
            pivot_x_ = 0.f;
            pivot_y_ = .5f;
        }
        else if (selected_pivot_mode == 5)
        {
            pivot_x_ = 1.f;
            pivot_y_ = .5f;
        }
        else if (selected_pivot_mode == 6)
        {
            pivot_x_ = 0.f;
            pivot_y_ = 0.f;
        }
        else if (selected_pivot_mode == 7)
        {
            pivot_x_ = .5f;
            pivot_y_ = 0.f;
        }
        else if (selected_pivot_mode == 8)
        {
            pivot_x_ = 1.f;
            pivot_y_ = 0.f;
        }
    }

    ImGui::Text("Pivot X: %.1f", pivot_x_);
    ImGui::Text("Pivot Y: %.1f", pivot_y_);
    
    if (ImGui::Button("Slice"))
    {
        int width = loaded_texture_->GetWidth();
        int height = loaded_texture_->GetHeight();
        
        float tile_size_x = width / grid[0];
        float tile_size_y = height / grid[1];

        frames_.clear();
        int index = 0;
        
        for (int y = 0; y < grid[1]; ++y)
        {
            for (int x = 0; x < grid[0]; ++x)
            {
                FrameData frame;
                frame.name = std::to_string(index++);
                frame.x = (x * tile_size_x) / width;
                frame.y = (y * tile_size_y) / height;
                frame.width = tile_size_x / width;
                frame.height = tile_size_y / height;
                frame.pivot_x = pivot_x_;
                frame.pivot_y = pivot_y_;

                frames_.push_back(frame);
            }
        }

        frame_names_.clear();
        for (const FrameData& frame : frames_)
        {
            frame_names_.push_back(frame.name.c_str());
        }
    }

    ImGui::Separator();
    
    if (loaded_texture_)
    {
        ImVec2 image_position = ImGui::GetCursorScreenPos();
        ImGui::Image(loaded_texture_->resource_view_.Get(), {loaded_texture_->GetWidth() * 1.f, loaded_texture_->GetHeight() * 1.f});

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRect(image_position, {image_position.x + loaded_texture_->GetWidth(), image_position.y + loaded_texture_->GetHeight()}, IM_COL32(255, 255, 255, 255));
        
        for (const FrameData& frame : frames_)
        {
            ImVec2 min = {image_position.x + frame.x * loaded_texture_->GetWidth(), image_position.y + frame.y * loaded_texture_->GetHeight()};
            ImVec2 max = {min.x + frame.width * loaded_texture_->GetWidth(), min.y + frame.height * loaded_texture_->GetHeight()};

            draw_list->AddRect(min, max, IM_COL32(0, 255, 0, 255));
        }
        
        for (const FrameData& frame : frames_)
        {
            ImVec2 min = {image_position.x + frame.x * loaded_texture_->GetWidth(), image_position.y + frame.y * loaded_texture_->GetHeight()};
            ImVec2 max = {min.x + frame.width * loaded_texture_->GetWidth(), min.y + frame.height * loaded_texture_->GetHeight()};

            draw_list->AddCircleFilled({min.x + frame.pivot_x * (max.x - min.x), min.y + (1 - frame.pivot_y) * (max.y - min.y)}, 3.f, IM_COL32(255, 0, 0, 255));
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
