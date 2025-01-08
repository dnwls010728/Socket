#pragma once
#include "Actor/Actor.h"
#include "Character/ContextSteering/IObstacle.h"

class Block : public Actor, public IObstacle
{
    SHADER_CLASS_HELPER(Block)
    GENERATED_BODY(Block, Actor)
    
public:
    Block(const std::wstring& kName);
    virtual ~Block() override = default;

    virtual ColliderComponent* GetCollider() const override;

private:
    class Sprite* sprite_;

    class SpriteRendererComponent* renderer_;
    class BoxColliderComponent* collider_;
    
};
