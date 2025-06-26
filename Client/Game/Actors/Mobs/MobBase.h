#pragma once
#include "Actors/NetworkActor.h"
#include "Actors/ServerActor.h"
#include "Subsystems/ObjectPool/IPoolable.h"

class MobBase : public ServerActor, public IPoolable
{
    SHADER_CLASS_HELPER(MobBase)
    GENERATED_BODY(MobBase, NetworkActor)
    
public:
    MobBase(const std::wstring& name);
    virtual ~MobBase() override = default;

    virtual void OnActivate() override;
    virtual void OnDeactivate() override;

protected:
    virtual void BeginPlay() override;
    virtual void OnEnable() override;
    virtual void OnDisable() override;
    
};
