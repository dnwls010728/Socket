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
        float x;
        float y;
        float width;
        float height;
        float pivot_x;
        float pivot_y;
    };

    void OpenTextureSettings(bool* is_open);
    void OpenTextureEditor(bool* is_open);

    bool show_texture_settings_;
    bool show_texture_editor_;

    int wrap_mode_;
    int filter_mode_;
    int ppu_;
    
    std::wstring file_path_;
    
    std::unique_ptr<Texture> loaded_texture_;

    std::vector<FrameData> frames_;
    
};
