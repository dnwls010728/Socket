#pragma once
#include "NetworkActor.h"

class DropItem : public NetworkActor
{
    SHADER_CLASS_HELPER(DropItem)
    GENERATED_BODY(DropItem, NetworkActor)
    
public:
    DropItem(const std::wstring& name);
    virtual ~DropItem() override = default;
    
};
