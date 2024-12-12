#include "pch.h"
#include "Editor.h"

#include <fstream>
#include <ShObjIdl.h>

#include "Logger.h"
#include "Data/FileHelper.h"
#include "imgui/imgui.h"
#include "Math/Math.h"
#include "Windows/DX/UITexture.h"
#include "yaml-cpp/yaml.h"

Editor::Editor(const std::wstring& kName) :
    Actor(kName),
    show_texture_settings_(true),
    show_texture_editor_(false),
    show_sprite_animator_(false),
    selected_wrap_mode_(0),
    selected_filter_mode_(0),
    ppu_(256),
    selected_frame_(0),
    selected_index_(0),
    selected_animation_(0),
    left_(0.f),
    top_(0.f),
    right_(1.f),
    bottom_(1.f),
    pivot_x_(0.f),
    pivot_y_(0.f),
    file_path_(L""),
    loaded_texture_(nullptr),
    frames_(),
    frame_indexes_(),
    animations_()
{
    std::strcpy(frame_name_, "");
}

void Editor::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    ImGui::DockSpaceOverViewport(0);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                World::Get()->OpenLevel(LevelType::kMainMenu);
            }

            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Texture Settings", nullptr, &show_texture_settings_);
            ImGui::MenuItem("Texture Editor", nullptr, &show_texture_editor_);
            ImGui::MenuItem("Sprite Animator", nullptr, &show_sprite_animator_);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (show_texture_settings_) OpenTextureSettings(&show_texture_settings_);
    if (show_texture_editor_) OpenTextureEditor(&show_texture_editor_);
    if (show_sprite_animator_) OpenSpriteAnimator(&show_sprite_animator_);
}

void Editor::OpenTextureSettings(bool* is_open)
{
    if (!ImGui::Begin("Texture Settings", is_open))
    {
        ImGui::End();
        return;
    }

    static const char* texture_types[] = {"Sprite", "User Interface"};
    static const char* wrap_modes[] = {"Repeat", "Clamp"};
    static const char* filter_modes[] = {"Point", "Bilinear"};

    static int selected_texture_type = 0;

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

                        selected_index_ = 0;
                        selected_animation_ = 0;
                        selected_frame_ = 0;

                        frame_indexes_.clear();
                        animations_.clear();
                        frames_.clear();
                        
                        ppu_ = 256;
                        
                        selected_filter_mode_ = 0;
                        selected_wrap_mode_ = 0;

                        if (loaded_texture_) loaded_texture_.reset();
                        loaded_texture_ = std::make_unique<Texture>();

                        if (!loaded_texture_->Load(file_path_))
                        {
                            loaded_texture_.reset();
                            file_path_ = L"";
                        }
                        else
                        {
                            std::string file_path_str(file_path_.begin(), file_path_.end());
                            YAML::Node node(YAML::NodeType::Null);

                            try
                            {
                                node = YAML::LoadFile(file_path_str + ".yaml");
                            }
                            catch (const YAML::BadFile& e)
                            {
                            }
                            
                            if (!node.IsNull())
                            {
                                selected_wrap_mode_ = node["wrap_mode"].as<int>();
                                selected_filter_mode_ = node["filter_mode"].as<int>();
                                ppu_ = node["ppu"].as<int>();

                                if (node["frames"].IsSequence())
                                {
                                    int i = 0;
                                    
                                    frames_.clear();
                                    for (const YAML::Node& frame : node["frames"])
                                    {
                                        FrameData data;
                                        data.name = frame["name"].as<std::string>();
                                        data.x = frame["rect"]["x"].as<float>();
                                        data.y = frame["rect"]["y"].as<float>();
                                        data.width = frame["rect"]["width"].as<float>();
                                        data.height = frame["rect"]["height"].as<float>();
                                        data.pivot_x = frame["pivot"]["x"].as<float>();
                                        data.pivot_y = frame["pivot"]["y"].as<float>();
                                        
                                        frames_.push_back(data);
                                    }
                                }
                            }
                        }

                        CoTaskMemFree(file_path);
                    }

                    item->Release();
                }
            }
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Save Texture"))
    {
        if (!loaded_texture_) return;

        std::wofstream file(file_path_ + L".yaml");
        YAML::Emitter emitter;

        emitter << YAML::BeginMap;
            emitter << YAML::Key << "wrap_mode";
            emitter << YAML::Value << selected_wrap_mode_;
            emitter << YAML::Key << "filter_mode";
            emitter << YAML::Value << selected_filter_mode_;
            emitter << YAML::Key << "ppu";
            emitter << YAML::Value << ppu_;
            emitter << YAML::Key << "frames";
            emitter << YAML::Value << YAML::BeginSeq;

            for (const FrameData& frame : frames_)
            {
                emitter << YAML::BeginMap;
                    emitter << YAML::Key << "name";
                    emitter << YAML::Value << frame.name;
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
    ImGui::Combo("Texture Type", &selected_texture_type, texture_types, IM_ARRAYSIZE(texture_types));
    ImGui::Combo("Wrap Mode", &selected_wrap_mode_, wrap_modes, IM_ARRAYSIZE(wrap_modes));
    ImGui::Combo("Filter Mode", &selected_filter_mode_, filter_modes, IM_ARRAYSIZE(filter_modes));

    if (selected_texture_type == 0)
    {
        ImGui::InputInt("PPU", &ppu_);

        if (ImGui::Button("Texture Editor")) show_texture_editor_ = true;

        ImGui::Text("Frames");
        bool is_selection_changed = ImGui::ListBox("##Frames", &selected_frame_, [](void* user_data, int index)
        {
            std::vector<FrameData>* frames = static_cast<std::vector<FrameData>*>(user_data);
            return frames->at(index).name.c_str();
        }, &frames_, frames_.size(), 4);

        if (is_selection_changed)
        {
            FrameData& frame = frames_[selected_frame_];
            std::strcpy(frame_name_, frame.name.c_str());
            
            left_ = frame.x;
            top_ = frame.y;
            right_ = frame.x + frame.width;
            bottom_ = frame.y + frame.height;
            pivot_x_ = frame.pivot_x;
            pivot_y_ = frame.pivot_y;
        }

        if (ImGui::Button("Add"))
        {
            FrameData frame;
            frame.name = "";
            frame.x = 0.f;
            frame.y = 0.f;
            frame.width = 1.f;
            frame.height = 1.f;
            frame.pivot_x = .5f;
            frame.pivot_y = .5f;

            frames_.push_back(frame);
        }

        ImGui::SameLine();

        if (ImGui::Button("Remove"))
        {
            if (!frames_.empty())
            {
                frames_.erase(frames_.begin() + selected_frame_);
                if (selected_frame_ >= frames_.size()) selected_frame_ = frames_.size() - 1;
                if (selected_frame_ < 0) selected_frame_ = 0;
            }
        }
    
        ImGui::Separator();
        ImGui::Text("Preview");
        if (ImGui::BeginChild("Preview", {200.f, 200.f}, true, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (loaded_texture_ && !frames_.empty())
            {
                ImVec2 uv0 = {frames_[selected_frame_].x, frames_[selected_frame_].y};
                ImVec2 uv1 = {frames_[selected_frame_].x + frames_[selected_frame_].width, frames_[selected_frame_].y + frames_[selected_frame_].height};

                float width = loaded_texture_->GetWidth();
                float height = loaded_texture_->GetHeight();

                ImGui::Image(loaded_texture_->resource_view_.Get(), {width, height}, uv0, uv1);
            }
        }

        ImGui::EndChild();
    }
    
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

    static const char* pivot_modes[] = {"Center", "Top Left", "Top", "Top Right", "Left", "Right", "Bottom Left", "Bottom", "Bottom Right", "Custom"};

    static int selected_pivot_mode = 0;

    static float auto_pivot_x = .5f;
    static float auto_pivot_y = .5f;

    static bool is_custom_pivot = false;

    ImGui::Text("Auto Slice");
    ImGui::InputInt2("Grid", grid);

    if (ImGui::Combo("Pivot Preset", &selected_pivot_mode, pivot_modes, IM_ARRAYSIZE(pivot_modes)))
    {
        if (selected_pivot_mode == 0)
        {
            auto_pivot_x = .5f;
            auto_pivot_y = .5f;
        }
        else if (selected_pivot_mode == 1)
        {
            auto_pivot_x = 0.f;
            auto_pivot_y = 1.f;
        }
        else if (selected_pivot_mode == 2)
        {
            auto_pivot_x = .5f;
            auto_pivot_y = 1.f;
        }
        else if (selected_pivot_mode == 3)
        {
            auto_pivot_x = 1.f;
            auto_pivot_y = 1.f;
        }
        else if (selected_pivot_mode == 4)
        {
            auto_pivot_x = 0.f;
            auto_pivot_y = .5f;
        }
        else if (selected_pivot_mode == 5)
        {
            auto_pivot_x = 1.f;
            auto_pivot_y = .5f;
        }
        else if (selected_pivot_mode == 6)
        {
            auto_pivot_x = 0.f;
            auto_pivot_y = 0.f;
        }
        else if (selected_pivot_mode == 7)
        {
            auto_pivot_x = .5f;
            auto_pivot_y = 0.f;
        }
        else if (selected_pivot_mode == 8)
        {
            auto_pivot_x = 1.f;
            auto_pivot_y = 0.f;
        }

        is_custom_pivot = selected_pivot_mode == 9;
    }

    if (is_custom_pivot)
    {
        ImGui::SetNextItemWidth(50.f);
        ImGui::InputFloat("Pivot X", &auto_pivot_x);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50.f);
        ImGui::InputFloat("Pivot Y", &auto_pivot_y);
    }
    
    if (ImGui::Button("Slice"))
    {
        if (grid[0] > 0 && grid[1] > 0)
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
                    std::wstring filename = FileHelper::GetFilenameWithoutExtension(file_path_);
                    std::string to_string = std::string(filename.begin(), filename.end());
                    
                    FrameData frame;
                    frame.name = to_string + "_" + std::to_string(index++);
                    frame.x = (x * tile_size_x) / width;
                    frame.y = (y * tile_size_y) / height;
                    frame.width = tile_size_x / width;
                    frame.height = tile_size_y / height;
                    frame.pivot_x = auto_pivot_x;
                    frame.pivot_y = auto_pivot_y;

                    frames_.push_back(frame);
                }
            }
        }
    }

    ImGui::Separator();
    ImGui::Text("Frame Settings");

    if (ImGui::InputText("Name", frame_name_, IM_ARRAYSIZE(frame_name_)))
    {
        frames_[selected_frame_].name = frame_name_;
    }
    
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("L", &left_))
    {
        frames_[selected_frame_].x = left_;
        frames_[selected_frame_].width = right_ - left_;
    }
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("T", &top_))
    {
        frames_[selected_frame_].y = top_;
        frames_[selected_frame_].height = bottom_ - top_;
    }
    
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("R", &right_))
    {
        frames_[selected_frame_].width = right_ - left_;
    }
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("B", &bottom_))
    {
        frames_[selected_frame_].height = bottom_ - top_;
    }

    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("PX", &pivot_x_))
    {
        frames_[selected_frame_].pivot_x = pivot_x_;
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("PY", &pivot_y_))
    {
        frames_[selected_frame_].pivot_y = pivot_y_;
    }
    
    ImGui::Separator();
    
    if (loaded_texture_)
    {
        float width = loaded_texture_->GetWidth();
        float height = loaded_texture_->GetHeight();
        
        ImVec2 image_position = ImGui::GetCursorScreenPos();
        ImGui::Image(loaded_texture_->resource_view_.Get(), {width * 1.f, height * 1.f});

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRect(image_position, {image_position.x + width, image_position.y + height}, IM_COL32(255, 255, 255, 255));
        
        for (const FrameData& frame : frames_)
        {
            ImVec2 min = {image_position.x + frame.x * width, image_position.y + frame.y * height};
            ImVec2 max = {min.x + frame.width * width, min.y + frame.height * height};

            draw_list->AddRect(min, max, IM_COL32(0, 255, 0, 255));
        }
        
        for (const FrameData& frame : frames_)
        {
            ImVec2 min = {image_position.x + frame.x * width, image_position.y + frame.y * height};
            ImVec2 max = {min.x + frame.width * width, min.y + frame.height * height};

            draw_list->AddCircleFilled({min.x + frame.pivot_x * (max.x - min.x), min.y + (1 - frame.pivot_y) * (max.y - min.y)}, 3.f, IM_COL32(255, 0, 0, 255));
        }

        if (!frames_.empty())
        {
            ImVec2 min = {image_position.x + frames_[selected_frame_].x * width, image_position.y + frames_[selected_frame_].y * height};
            ImVec2 max = {min.x + frames_[selected_frame_].width * width, min.y + frames_[selected_frame_].height * height};

            draw_list->AddRect(min, max, IM_COL32(255, 255, 255, 255), 0.f, 15, 3.f);
        }

        if (ImGui::IsItemHovered())
        {
            ImVec2 mouse_position = ImGui::GetMousePos();
            ImVec2 uv = {(mouse_position.x - image_position.x) / width, (mouse_position.y - image_position.y) / height};

            if (ImGui::IsMouseClicked(0))
            {
                for (auto it = frames_.rbegin(); it != frames_.rend(); ++it)
                {
                    const FrameData& frame = *it;
                    if (uv.x >= frame.x && uv.x <= frame.x + frame.width && uv.y >= frame.y && uv.y <= frame.y + frame.height)
                    {
                        selected_frame_ = std::distance(it, frames_.rend()) - 1;
                        left_ = frame.x;
                        top_ = frame.y;
                        right_ = frame.x + frame.width;
                        bottom_ = frame.y + frame.height;
                        pivot_x_ = frame.pivot_x;
                        pivot_y_ = frame.pivot_y;
                        break;
                    }
                }
            }

            ImGui::BeginTooltip();
            ImGui::Text("UV: (%.2f, %.2f)", uv.x, uv.y);
            ImGui::EndTooltip();
        }
    }

    ImGui::End();
}

void Editor::OpenSpriteAnimator(bool* is_open)
{
    if (!ImGui::Begin("Sprite Animator", is_open))
    {
        ImGui::End();
        return;
    }

    static int sample_frame_rate = 60;
    static int scale = 1.f;
    static int current_frame = 0;

    static bool is_repeat = false;
    static bool is_playing = false;

    static float timer = 0.f;

    static float x = 0.f;
    static float y = 0.f;

    static char animation_name[256] = "";

    ImGui::Text("Sample Frame Rate");
    ImGui::InputInt("##Sample Frame Rate", &sample_frame_rate);
    ImGui::SameLine();
    ImGui::BeginDisabled(frame_indexes_.empty());
    if (ImGui::Button(is_playing ? "Pause" : "Play"))
    {
        is_playing = !is_playing;
        timer = 0.f;
    }
    
    ImGui::EndDisabled();

    ImVec2 start_position = ImGui::GetCursorScreenPos();
    if (ImGui::BeginChild("Sprite Animator", {300.f, 300.f}, true, ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImVec2 cursor_center = {start_position.x + 150.f + x, start_position.y + 150.f + y};
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        if (loaded_texture_ && !frame_indexes_.empty())
        {
            float frame_x = frames_[current_frame].x;
            float frame_y = frames_[current_frame].y;
            float frame_width = frames_[current_frame].width;
            float frame_height = frames_[current_frame].height;
            
            ImVec2 uv0 = {frame_x, frame_y};
            ImVec2 uv1 = {frame_x + frame_width, frame_y + frame_height};

            float width = loaded_texture_->GetWidth() * frame_width;
            float height = loaded_texture_->GetHeight() * frame_height;

            width *= scale;
            height *= scale;

            float pivot_x = width * frames_[current_frame].pivot_x;
            float pivot_y = height * (1.f - frames_[current_frame].pivot_y);

            ImVec2 center_position = {150.f + x, 150.f + y};
            ImVec2 pivot_position = {center_position.x - pivot_x, center_position.y - pivot_y};

            ImGui::SetCursorPos(pivot_position);
            ImGui::Image(loaded_texture_->resource_view_.Get(), {width, height}, uv0, uv1);
        }

        float length = Math::Clamp(10.f * scale, 10.f, 10.f * scale);
        
        draw_list->AddLine(cursor_center, {cursor_center.x + length, cursor_center.y}, IM_COL32(255, 0, 0, 255));
        draw_list->AddLine(cursor_center, {cursor_center.x, cursor_center.y - length}, IM_COL32(0, 255, 0, 255));
    }

    ImGui::EndChild();
    
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsItemHovered())
    {
        if (io.MouseWheel > 0)
        {
            scale += 1;
        }
        else if (io.MouseWheel < 0)
        {
            scale = Math::Max(1.f, scale - 1);
        }
        
        if (ImGui::IsMouseDragging(1))
        {
            ImVec2 delta = io.MouseDelta;
            x += delta.x;
            y += delta.y;
        }
    }
    
    ImGui::SameLine();
    
    ImGui::BeginGroup();
    ImGui::Text("Frame Indexes");
    
    ImGui::SetNextItemWidth(100.f);
    bool is_selection_changed = ImGui::ListBox("##Frame Indexes", &selected_index_, [](void* user_data, int index)
    {
        std::vector<std::string>* frame_indexes = static_cast<std::vector<std::string>*>(user_data);
        return frame_indexes->at(index).c_str();
    }, &frame_indexes_, frame_indexes_.size(), 4);

    if (is_selection_changed)
    {
        const auto it = std::ranges::find_if(frames_, [this](const FrameData& frame)
        {
            return frame.name == frame_indexes_[selected_index_];
        });

        current_frame = std::distance(frames_.begin(), it);
    }

    ImGui::BeginDisabled(is_playing);
    if (ImGui::Button("Add##Frame Index"))
    {
        frame_indexes_.push_back(frames_[selected_frame_].name);
    }

    ImGui::EndDisabled();
    ImGui::SameLine();

    ImGui::BeginDisabled(is_playing);
    if (ImGui::Button("Remove##Frame Index"))
    {
        if (!frame_indexes_.empty())
        {
            frame_indexes_.erase(frame_indexes_.begin() + selected_index_);
            selected_index_ = Math::Clamp(selected_index_, 0, frame_indexes_.size() - 1);
        }
    }

    ImGui::EndDisabled();
    
    ImGui::BeginDisabled(is_playing);
    ImGui::Checkbox("Repeat", &is_repeat);
    ImGui::EndDisabled();

    ImGui::Separator();
    ImGui::Text("View Settings");

    if (ImGui::Button("Center"))
    {
        x = 0.f;
        y = 0.f;
    }

    ImGui::SameLine();

    if (ImGui::Button("Default Size"))
    {
        scale = 1.f;
    }
    
    if (ImGui::Button("Reset"))
    {
        x = 0.f;
        y = 0.f;

        scale = 1.f;
    }
    
    ImGui::EndGroup();

    ImGui::Text("Animations");
    is_selection_changed = ImGui::ListBox("##Animations", &selected_animation_, [](void* user_data, int index)
    {
        std::vector<AnimationData>* animations = static_cast<std::vector<AnimationData>*>(user_data);
        return animations->at(index).name.c_str();
    }, &animations_, animations_.size(), 4);

    if (is_selection_changed)
    {
        selected_index_ = 0;
        sample_frame_rate = animations_[selected_animation_].sample_frame_rate;
        is_repeat = animations_[selected_animation_].is_repeat;
        
        frame_indexes_.clear();
        for (const auto& index : animations_[selected_animation_].frame_indexes)
        {
            frame_indexes_.push_back(index);
        }
    }

    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Text("Animation Name");
    
    ImGui::BeginDisabled(is_playing);
    ImGui::SetNextItemWidth(100.f);
    ImGui::InputText("##Animation Name", animation_name, IM_ARRAYSIZE(animation_name));
    ImGui::EndDisabled();
    
    ImGui::BeginDisabled(is_playing);
    if (ImGui::Button("Add##Animation"))
    {
        selected_index_ = 0;
        
        AnimationData animation;
        animation.name = animation_name;
        animation.sample_frame_rate = sample_frame_rate;
        animation.is_repeat = is_repeat;

        for (const std::string& index : frame_indexes_)
        {
            animation.frame_indexes.push_back(index);
        }

        animations_.push_back(animation);

        std::strcpy(animation_name, "");
    }

    ImGui::EndDisabled();
    ImGui::SameLine();

    ImGui::BeginDisabled(is_playing);
    if (ImGui::Button("Remove##Animation"))
    {
        if (!animations_.empty())
        {
            animations_.erase(animations_.begin() + selected_animation_);
            if (selected_animation_ >= animations_.size()) selected_animation_ = animations_.size() - 1;
            if (selected_animation_ < 0) selected_animation_ = 0;
        }
    }
    
    ImGui::EndDisabled();
    ImGui::EndGroup();
    
    if (ImGui::Button("Save Animations"))
    {
    }
    
    ImGui::End();
    
    if (is_playing)
    {
        timer += ImGui::GetIO().DeltaTime;
        if (timer >= 1.f / sample_frame_rate)
        {
            timer = 0.f;
            selected_index_ = (selected_index_ + 1) % frame_indexes_.size();
            
            const auto it = std::ranges::find_if(frames_, [this](const FrameData& frame)
            {
                return frame.name == frame_indexes_[selected_index_];
            });

            current_frame = std::distance(frames_.begin(), it);
        }
    }
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
