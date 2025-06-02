#pragma once

namespace FSM
{
    class IState
    {
    public:
        virtual ~IState() = default;

        virtual void Enter() = 0;
        virtual void Tick(float delta_time) = 0;
        virtual void Exit() = 0;
        
    };
}
