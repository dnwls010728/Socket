#pragma once
#include "Character/Mob/MobBase.h"

class KnightRun;
class KnightIdle;

class Knight : public MobBase
{
    SHADER_CLASS_HELPER(Knight)
    GENERATED_BODY(Knight, MobBase)
    
public:
    Knight(const std::wstring& kName);
    virtual ~Knight() override = default;

    FORCEINLINE KnightIdle* GetIdleState() const { return idle_state_.get(); }
    FORCEINLINE KnightRun* GetRunState() const { return run_state_.get(); }

private:
    class AnimationPack* animation_pack_;

    std::shared_ptr<KnightIdle> idle_state_;
    std::shared_ptr<KnightRun> run_state_;
    
};
