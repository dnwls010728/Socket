#pragma once
#include "Actor/Actor.h"

class BlockPreview : public Actor
{
    SHADER_CLASS_HELPER(BlockPreview)
    GENERATED_BODY(BlockPreview, Actor)
    
public:
    BlockPreview(const std::wstring& name);
    virtual ~BlockPreview() override = default;

private:
    std::shared_ptr<SpriteRendererComponent> renderer_;
    
};
