#include "pch.h"
#include "Mob1Idle.h"

#include "Character/Component/FSM/StateMachine.h"

Mob1Idle::Mob1Idle(StateMachine* state_machine) :
    State(state_machine)
{
}

void Mob1Idle::Enter()
{
}

void Mob1Idle::Tick(float delta_time)
{
}

void Mob1Idle::Exit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<Mob1Idle>("Mob1Idle")
        .constructor<StateMachine*>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
