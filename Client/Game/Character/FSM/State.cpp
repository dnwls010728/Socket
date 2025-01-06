#include "pch.h"
#include "State.h"

State::State(StateMachine* state_machine) :
    state_machine_(state_machine)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<State>("State");
}
