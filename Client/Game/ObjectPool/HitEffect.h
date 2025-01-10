#pragma once
#include "PooledObject.h"

class HitEffect : public PooledObject
{
    SHADER_CLASS_HELPER(HitEffect)
    GENERATED_BODY(HitEffect, PooledObject)
    
public:
    HitEffect(const std::wstring& kName);
    virtual ~HitEffect() override = default;

protected:
    virtual void OnEnable() override;
    
    class Sprite* sprite_;
    class AnimationPack* animation_pack_;
    
    class SpriteRendererComponent* renderer_;
    class AnimatorComponent* animator_;

private:
    void OnPlayEnd();
    
};
