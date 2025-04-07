#pragma once

namespace FSM
{
    class IState
    {
    public:
        virtual ~IState() = default;

        virtual void Enter() = 0;
        virtual void PhysicsTick() = 0;
        virtual void Tick() = 0;
        virtual void PostTick() = 0;
        virtual void Exit() = 0;
        
    };
}
