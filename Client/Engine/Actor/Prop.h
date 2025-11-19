#pragma once
#include "Actor.h"

class Prop : public Actor
{
    SHADER_CLASS_HELPER(Prop)
    GENERATED_BODY(Prop, Actor)
    
public:
    Prop(const std::wstring& name);
    virtual ~Prop() override = default;

    void SetSprite(const std::wstring& path, int32_t frame_index, int32_t order = 0) const;

protected:
    virtual void Render(float alpha) override;
    
    std::shared_ptr<SpriteRendererComponent> renderer_;
    
};
