#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Windows/DX/Sprite.h"

class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    class SpriteRendererComponent* renderer_;

    float timer_;

    int current_index_;
    
};
