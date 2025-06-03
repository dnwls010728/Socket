#pragma once
#include "PlayerState.h"

class PlayerIdleState : public PlayerState
{
public:
    PlayerIdleState(const std::shared_ptr<PlayerCharacter>& player_character);
    virtual ~PlayerIdleState() override = default;
    
};
