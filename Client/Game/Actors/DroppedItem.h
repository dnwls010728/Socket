#pragma once
#include "NetworkActor.h"
#include "Subsystems/ObjectPool/IPoolable.h"

class SpriteRendererComponent;
class BoxColliderComponent;
class Controller2DComponent;

class DroppedItem : public NetworkActor, public IPoolable
{
    SHADER_CLASS_HELPER(DroppedItem)
    GENERATED_BODY(DroppedItem, NetworkActor)
    
public:
    DroppedItem(const std::wstring& name);
    virtual ~DroppedItem() override = default;
    
    virtual void OnActivate() override;
    virtual void OnDeactivate() override;

    void Init(uint32_t item_id, const Math::Vector2& drop_position);

protected:
    enum class State
    {
        kDropped,
        kFloating,
        kPickedUp
    };
    
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;

    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<Controller2DComponent> controller_;

    State current_state_;

    Math::Vector2 start_position_;
    Math::Vector2 drop_position_;
    Math::Vector2 velocity_;

    float timer_;
    
};
