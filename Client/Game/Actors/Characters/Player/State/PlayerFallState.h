#pragma once
#include "PlayerState.h"

class PlayerFallState : public PlayerState
{
public:
    PlayerFallState(const std::shared_ptr<PlayerCharacter>& player_character);
    virtual ~PlayerFallState() override = default;

    virtual void Enter() override;
    virtual void Exit() override;
    
};
