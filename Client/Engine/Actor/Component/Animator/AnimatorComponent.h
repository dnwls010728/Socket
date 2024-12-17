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

    void PlayAnimation(const std::wstring& kName);

    FORCEINLINE void SetAnimationPack(AnimationPack* animation_pack) { animation_pack_ = animation_pack; }

    FORCEINLINE bool IsPlaying() const { return is_playing_; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    class SpriteRendererComponent* renderer_;
    
    AnimationPack* animation_pack_;

    class Animation* current_animation_;

    float timer_;

    bool is_playing_;

    int current_frame_;
    
};
