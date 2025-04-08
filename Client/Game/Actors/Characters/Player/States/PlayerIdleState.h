#pragma once
#include "Actors/Characters/States/BaseState.h"

class PlayerIdleState : public BaseState
{
public:
    PlayerIdleState(const std::shared_ptr<CharacterBase>& kCharacter);
    virtual ~PlayerIdleState() override = default;
    
};
