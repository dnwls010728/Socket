#include "pch.h"
#include "PlayerDashState.h"

PlayerDashState::PlayerDashState(const std::shared_ptr<CharacterBase>& kCharacter) 
    : BaseState(kCharacter)
{
}

void PlayerDashState::Enter()
{
    BaseState::Enter();

    Logger::Print(L"PlayerDashState::Enter()");
}
