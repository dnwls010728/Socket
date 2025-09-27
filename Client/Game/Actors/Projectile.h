#pragma once

#include "Actors/ServerActor.h"
#include "Subsystems/ObjectPool/IPoolable.h"

class Projectile : public ServerActor, public IPoolable
{
    SHADER_CLASS_HELPER(Projectile)
    GENERATED_BODY(Projectile, ServerActor)

public:
    Projectile(const std::wstring& name);
    virtual ~Projectile() override = default;

    void Init(uint32_t projectile_id);

    virtual void OnActivate() override;
    virtual void OnDeactivate() override;

protected:
    void OnEnable() override;
    void OnDisable() override;

private:
    uint32_t projectile_id_;
};

