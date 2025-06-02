#pragma once
#include "Actors/NetworkActor.h"

class AnimatorComponent;

namespace UI_OLD
{
    class NameTag;
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

    void SetCharacterName(const std::wstring& name);
    void Speak(const std::wstring& message, float duration = 4.f);
    
    FORCEINLINE const std::wstring& GetCharacterName() const { return character_name_; }
    FORCEINLINE std::shared_ptr<SpriteRendererComponent> GetRenderer() const { return renderer_; }
    FORCEINLINE std::shared_ptr<StateMachineComponent> GetStateMachine() const { return state_machine_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void EndPlay(EndPlayReason type) override;
    
    virtual void OnSpeakEnd();
    
#pragma region 컴포넌트
    std::shared_ptr<BoxColliderComponent> collider_;
    std::shared_ptr<Controller2DComponent> controller_;
    std::shared_ptr<SpriteRendererComponent> renderer_;
    std::shared_ptr<AnimatorComponent> animator_;
    std::shared_ptr<StateMachineComponent> state_machine_;
#pragma endregion

    std::wstring character_name_;

    Math::Vector2 velocity_;

    float gravity_;
    
#pragma region UI
    std::shared_ptr<UI_OLD::NameTag> name_tag_;
    std::shared_ptr<UI_OLD::ChatBalloon> chat_balloon_;
#pragma endregion

    TimerHandle chat_balloon_timer_handle_;
    
};
