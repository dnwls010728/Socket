#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

class StateMachineComponent;
class Controller2DComponent;
class CharacterBase : public Actor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, Actor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    inline void SetMovementInput(const Math::Vector2& movement_input){movement_input_ = movement_input;}
protected:
    Math::Vector2 velocity_;
    float gravity_;
    Math::Vector2 movement_input_;
    
    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<Controller2DComponent> controller_;
    std::shared_ptr<StateMachineComponent> state_machine_;
};
