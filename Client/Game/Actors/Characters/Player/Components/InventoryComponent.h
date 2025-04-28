#pragma once
#include "Actor/Component/ActorComponent.h"

class InventoryComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(InventoryComponent)
    GENERATED_BODY(InventoryComponent, ActorComponent)
    
public:
    InventoryComponent(Actor* owner, const std::wstring& name);
    virtual ~InventoryComponent() override = default;
    
};
