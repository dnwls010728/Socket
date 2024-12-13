#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Asset/AnimationSet.h"
#include "Windows/DX/Sprite.h"

class AnimationSet;

class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

    void SetAnimationSet(AnimationSet* animation_set);
    void PlayAnimation(const std::wstring& kSequence);

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    class SpriteRendererComponent* renderer_;

    float timer_;

    int current_index_;

    bool is_playing_;

    AnimationSet* animation_set_;
    
    AnimationSequence current_sequence_;
    
};
