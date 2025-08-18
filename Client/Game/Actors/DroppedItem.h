#pragma once
#include "NetworkActor.h"
#include "Subsystems/ObjectPool/IPoolable.h"

class AnimatorComponent;
class PlayerCharacter;
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

    void Pickup(const std::shared_ptr<Actor>& character);
    
    virtual void OnActivate() override;
    virtual void OnDeactivate() override;

    void Init(uint32_t item_id, int32_t color, const Math::Vector2& drop_position) const;

protected:
    virtual void OnEnable() override;
    virtual void OnDisable() override;

    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<AnimatorComponent> animator_;
    std::shared_ptr<BoxColliderComponent> collider_;
    
};
