#pragma once
#include "PlayerStateBase.h"

class PlayerDeathState : public PlayerStateBase
{
public:
    PlayerDeathState(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator);
    virtual ~PlayerDeathState() override = default;

    virtual void Enter() override;
    
};
