#pragma once
#include "Actor/Component/ActorComponent.h"

class AnimationPack;

class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

private:
    AnimationPack* animation_pack_;
    
};
