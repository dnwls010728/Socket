#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

class SpriteRendererComponent;

class Weapon : public Actor
{
    SHADER_CLASS_HELPER(Weapon)
    GENERATED_BODY(Weapon, Actor)
    
public:
    Weapon(const std::wstring& kName);
    virtual ~Weapon() override = default;

    void Shot(const Math::Vector2& kDirection);

    FORCEINLINE SpriteRendererComponent* GetRenderer() const { return renderer_; }

private:
    SpriteRendererComponent* renderer_;
    class ObjectPool* bullet_pool_;
    
    class Sprite* sprite_;
    
};
