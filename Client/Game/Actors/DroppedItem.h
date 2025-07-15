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
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;

    float ApproxBezierLength(const Math::Vector2& p1, const Math::Vector2& p2, const Math::Vector2& p3, int32_t steps = 100);

    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<BoxColliderComponent> collider_;

    Math::Vector2 start_position_;
    Math::Vector2 drop_position_;
    Math::Vector2 control_;

    float timer_;
    float duration_;
    
};
