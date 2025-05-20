#pragma once
#include "BaseState.h"
#include "FSM/IState.h"

class IdleState : public BaseState
{
public:
    IdleState(const std::shared_ptr<CharacterBase>& kCharacter) : BaseState(kCharacter) {};
};
