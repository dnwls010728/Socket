#pragma once
#include "Actors/NetworkActor.h"

namespace UI
{
    class ChatBalloon;
}

class Controller2DComponent;
class StateMachineComponent;

class CharacterBase : public NetworkActor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, NetworkActor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    void Speak(const std::wstring& message);

    FORCEINLINE std::shared_ptr<SpriteRendererComponent> GetRenderer() const { return renderer_; }
    FORCEINLINE std::shared_ptr<StateMachineComponent> GetStateMachine() const { return state_machine_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void EndPlay(EndPlayReason type) override;
    
#pragma region 컴포넌트
    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<Controller2DComponent> controller_;
    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<StateMachineComponent> state_machine_;
#pragma endregion

    Math::Vector2 velocity_;

    float gravity_;
    
#pragma region UI
    std::shared_ptr<UI::ChatBalloon> chat_balloon_;
#pragma endregion

    TimerHandle chat_balloon_timer_handle_;
    
};
