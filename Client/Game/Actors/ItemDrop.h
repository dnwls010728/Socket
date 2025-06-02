#pragma once
#include "NetworkActor.h"

class SpriteRendererComponent;
class BoxColliderComponent;
class Controller2DComponent;

class ItemDrop : public NetworkActor
{
    SHADER_CLASS_HELPER(ItemDrop)
    GENERATED_BODY(ItemDrop, NetworkActor)
    
public:
    ItemDrop(const std::wstring& name);
    virtual ~ItemDrop() override = default;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    
    enum class State
    {
        kDropped,
        kFloating,
        kPickedUp
    };

    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<Controller2DComponent> controller_;

    float gravity_;
    float base_y_;
    float moved_;
    
    Math::Vector2 velocity_;

    State current_state_;
    
};
