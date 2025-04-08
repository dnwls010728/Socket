#include "pch.h"
#include "Condition.h"

FSM::Condition::Condition(bool(* func)()) :
    function_(func)
{
}

bool FSM::Condition::Evaluate()
{
    return function_();
}
