#pragma once
#include "Actor/Actor.h"

class SpriteRendererComponent;

class Weapon : public Actor
{
    SHADER_CLASS_HELPER(Weapon)
    GENERATED_BODY(Weapon, Actor)
    
public:
    Weapon(const std::wstring& kName);
    virtual ~Weapon() override = default;

    FORCEINLINE SpriteRendererComponent* GetRenderer() const { return renderer_; }

protected:
    virtual void Tick(float delta_time) override;

private:
    SpriteRendererComponent* renderer_;
    class ObjectPool* bullet_pool_;
    
    class Sprite* sprite_;
    
};
