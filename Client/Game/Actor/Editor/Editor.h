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
        std::wstring name;
    };
    
    std::wstring OpenFile();
    
    std::wstring file_path_;
    
    std::unique_ptr<Texture> loaded_texture_;

    std::vector<FrameData> frames_;
    
};
