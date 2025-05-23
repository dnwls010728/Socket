#pragma once
#include "BaseState.h"
#include "Actors/Character/CharacterBase.h"

class MoveToPlayerState : public BaseState
{
public:
    MoveToPlayerState(const std::shared_ptr<CharacterBase>& kCharacter);

    virtual void PhysicsTick(float);
};
