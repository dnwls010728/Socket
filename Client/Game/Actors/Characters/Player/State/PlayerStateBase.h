#pragma once
#include "FSM/IState.h"

class AnimatorComponent;
class StateMachineComponent;
class PlayerCharacter;

class PlayerStateBase : public FSM::IState
{
public:
    PlayerStateBase(const std::shared_ptr<PlayerCharacter>& owner, const std::shared_ptr<AnimatorComponent>& animator);
    virtual ~PlayerStateBase() override = default;
    
    virtual void Enter() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void PostTick(float delta_time) override;
    virtual void Exit() override;

protected:
    std::weak_ptr<PlayerCharacter> owner_;
    std::weak_ptr<AnimatorComponent> animator_;
    
};
