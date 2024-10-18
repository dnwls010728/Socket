#pragma once
#include "Actor/Actor.h"

class Platform : public Actor
{
    SHADER_CLASS_HELPER(Platform)
    GENERATED_BODY(Platform, Actor)
    
public:
    Platform(const std::wstring& kName);
    virtual ~Platform() override = default;

protected:
    virtual void Tick(float delta_time) override;

private:
    class SpriteRendererComponent* renderer_;
    class BoxColliderComponent* collider_;
    class PlatformControllerComponent* controller_;

    class Sprite* sprite_;
    
};
