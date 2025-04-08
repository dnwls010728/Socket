#pragma once
#include "Actors/Characters/States/BaseState.h"

class PlayerDashState : public BaseState
{
public:
    PlayerDashState(const std::shared_ptr<CharacterBase>& kCharacter);
    virtual ~PlayerDashState() override = default;

    virtual void Enter() override;
    
};
