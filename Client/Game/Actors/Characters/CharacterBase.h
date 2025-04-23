#pragma once
#include "Actors/NetworkActor.h"

class Controller2D;
class StateMachineComponent;

class CharacterBase : public NetworkActor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, NetworkActor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    FORCEINLINE std::shared_ptr<SpriteRendererComponent> GetRenderer() const { return renderer_; }
    FORCEINLINE std::shared_ptr<StateMachineComponent> GetStateMachine() const { return state_machine_; }

protected:
#pragma region 컴포넌트
    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<Controller2D> controller_;
    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<StateMachineComponent> state_machine_;
#pragma endregion

    Math::Vector2 velocity_;

    float gravity_;
    
};
