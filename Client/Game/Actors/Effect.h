#pragma once
#include "NetworkActor.h"

class AnimatorComponent;

class Effect : public NetworkActor
{
    SHADER_CLASS_HELPER(Effect)
    GENERATED_BODY(Effect, NetworkActor)

public:
    Effect(const std::wstring& name);
    virtual ~Effect() override = default;

    void SetFlipX(bool flipX);
    void SetAnimationPack(const std::wstring& animation_pack) { animation_pack_ = animation_pack; }
    void SetAnimation(const std::wstring& animation) { animation_ = animation; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<AnimatorComponent> animator_;

    std::wstring animation_pack_;
    std::wstring animation_;
};
