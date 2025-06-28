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
    
    FORCEINLINE bool IsDead() const { return is_dead_; }

protected:
    virtual void OnEnable() override;
    virtual void OnDisable() override;

    bool is_dead_;

    float fade_timer_;
    
};
