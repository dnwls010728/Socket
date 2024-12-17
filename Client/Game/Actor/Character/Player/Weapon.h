#pragma once
#include "Actor/Actor.h"

class Weapon : public Actor
{
    SHADER_CLASS_HELPER(Weapon)
    GENERATED_BODY(Weapon, Actor)
    
public:
    Weapon(const std::wstring& kName);
    virtual ~Weapon() override = default;

protected:
    virtual void Tick(float delta_time) override;

private:
    class Sprite* sprite_;
    
    class SpriteRendererComponent* renderer_;
    
};
