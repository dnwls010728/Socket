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
protected:
    Math::Vector2 velocity_;
    float gravity_;

    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<Controller2DComponent> controller_;
    std::shared_ptr<StateMachineComponent> state_machine_;
};
