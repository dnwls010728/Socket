#pragma once
#include "Actors/NetworkActor.h"
#include "Actors/ServerActor.h"

class MobBase : public ServerActor
{
    SHADER_CLASS_HELPER(MobBase)
    GENERATED_BODY(MobBase, NetworkActor)
    
public:
    MobBase(const std::wstring& name);
    virtual ~MobBase() override = default;

protected:
    virtual void BeginPlay() override;
    
};
