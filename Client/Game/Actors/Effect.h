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

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<AnimatorComponent> animator_;
    
};
