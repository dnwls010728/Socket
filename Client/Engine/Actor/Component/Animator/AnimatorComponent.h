#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Misc/DelegateMacros.h"

DECLARE_DELEGATE(OnAnimationDelegate)

class Animation;
class AnimationPack;

class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

    void PlayAnimation(const std::wstring& kName);

    FORCEINLINE void SetAnimationPack(AnimationPack* animation_pack) { animation_pack_ = animation_pack; }

    FORCEINLINE Animation* GetCurrentAnimation() const { return current_animation_; }

    FORCEINLINE bool IsPlaying() const { return is_playing_; }

    OnAnimationDelegate OnEndHandler;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    class SpriteRendererComponent* renderer_;
    
    AnimationPack* animation_pack_;

    Animation* current_animation_;

    float timer_;

    bool is_playing_;

    int current_frame_;
    
};
