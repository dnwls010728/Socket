#pragma once
#include "NetworkActor.h"

class Controller2DComponent;

class ItemDrop : public NetworkActor
{
    SHADER_CLASS_HELPER(ItemDrop)
    GENERATED_BODY(ItemDrop, NetworkActor)
    
public:
    ItemDrop(const std::wstring& name);
    virtual ~ItemDrop() override = default;

protected:
    virtual void Tick(float delta_time) override;

private:
    std::shared_ptr<Controller2DComponent> controller_;

    float gravity_;
    
    Math::Vector2 velocity_;
    
};
