#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Math/Vector2.h"

class PlayerController : public ActorComponent
{
    SHADER_CLASS_HELPER(PlayerController)
    GENERATED_BODY(PlayerController, ActorComponent)
    
public:
    PlayerController(Actor* owner, const std::wstring& kName);
    virtual ~PlayerController() override = default;
    
    FORCEINLINE const Math::Vector2& GetMovementInput() const { return movement_input_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTickComponent(float delta_time) override;
    virtual void TickComponent(float delta_time) override;

private:
    class PlayerCharacter* character_;

    std::weak_ptr<class StateMachineComponent> state_machine_;
    
    Math::Vector2 movement_input_;
    
};
