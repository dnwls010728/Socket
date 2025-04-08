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
    virtual void TickComponent(float delta_time) override;

    void UpdateInteraction();

private:
    class PlayerCharacter* character_;

    std::weak_ptr<class StateMachineComponent> state_machine_;
    
    Math::Vector2 movement_input_;
    Math::Vector2 mouse_direction_;

    float mouse_direction_angle_;
    float mouse_distance_;

    TimerHandle interaction_timer_;
    
};
