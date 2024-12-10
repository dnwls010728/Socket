#pragma once
#include "Actor/Actor.h"
#include "Windows/DX/Texture.h"

class Editor : public Actor
{
    SHADER_CLASS_HELPER(Editor)
    GENERATED_BODY(Editor, Actor)
    
public:
    Editor(const std::wstring& kName);
    virtual ~Editor() override = default;

protected:
    virtual void Tick(float delta_time) override;

private:
    struct FrameData
    {
        std::string index;
        
        float x;
        float y;
        float width;
        float height;
        float pivot_x;
        float pivot_y;
    };

    struct AnimationData
    {
        std::string name;
        float sample_frame_rate;
        bool is_repeat;
        std::vector<Type::uint32> frame_indexes;
    };

    void OpenTextureSettings(bool* is_open);
    void OpenTextureEditor(bool* is_open);
    void OpenSpriteAnimator(bool* is_open);

    bool show_texture_settings_;
    bool show_texture_editor_;
    bool show_sprite_animator_;

    int selected_wrap_mode_;
    int selected_filter_mode_;
    int ppu_;
    int selected_frame_;
    int selected_index_;
    int selected_animation_;

    float left_;
    float top_;
    float right_;
    float bottom_;
    float pivot_x_;
    float pivot_y_;
    
    std::wstring file_path_;
    
    std::unique_ptr<Texture> loaded_texture_;

    std::vector<FrameData> frames_;
    std::vector<std::string> frame_indexes_;
    std::vector<AnimationData> animations_;
    
};
