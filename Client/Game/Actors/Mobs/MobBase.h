#pragma once
#include "Actors/NetworkActor.h"
#include "Actors/ServerActor.h"
#include "Subsystems/ObjectPool/IPoolable.h"

class Audio;

class MobBase : public ServerActor, public IPoolable
{
    SHADER_CLASS_HELPER(MobBase)
    GENERATED_BODY(MobBase, NetworkActor)
    
public:
    MobBase(const std::wstring& name);
    virtual ~MobBase() override = default;

    virtual void OnActivate() override;
    virtual void OnDeactivate() override;
    
    virtual void OnDeath();

    void Init(uint32_t mob_id) const;
    
    FORCEINLINE bool IsDead() const { return is_dead_; }
    FORCEINLINE bool IsFading() const { return fade_state_ != FadeState::kNone; }

protected:
    enum class FadeState : uint8_t
    {
        kNone,
        kFadeIn,
        kFadeOut
    };
    
    virtual void OnEnable() override;
    virtual void OnDisable() override;
    virtual void Tick(float delta_time) override;

    bool is_dead_;

    FadeState fade_state_;

    float fade_timer_;
    
};
