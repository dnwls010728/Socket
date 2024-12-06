#include "pch.h"
#include "PlayerIdle.h"

#include "Actor/NetworkActor.h"
#include "Character/Component/FSM/StateMachine.h"

PlayerIdle::PlayerIdle(StateMachine* state_machine) :
    State(state_machine)
{
}

void PlayerIdle::Enter()
{
}

void PlayerIdle::Tick(float delta_time)
{
}

void PlayerIdle::Exit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerIdle>("PlayerIdle")
        .constructor<StateMachine*>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
