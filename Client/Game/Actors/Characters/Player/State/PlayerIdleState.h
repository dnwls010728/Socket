#pragma once
#include "PlayerStateBase.h"

class PlayerIdleState : public PlayerStateBase
{
public:
    PlayerIdleState(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator);
    virtual ~PlayerIdleState() override = default;

    virtual void Enter() override;
    
};
