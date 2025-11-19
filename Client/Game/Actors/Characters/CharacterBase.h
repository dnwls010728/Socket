#pragma once
#include "Actors/ServerActor.h"

class UIChatBalloon;
class UINameTag;
class AnimatorComponent;

class Controller2DComponent;
class StateMachineComponent;

class CharacterBase : public ServerActor
{
    SHADER_CLASS_HELPER(CharacterBase)
    GENERATED_BODY(CharacterBase, ServerActor)
    
public:
    CharacterBase(const std::wstring& kName);
    virtual ~CharacterBase() override = default;

    void SetCharacterName(const std::wstring& name);
    void Speak(const std::wstring& message, float duration = 4.f);
    
    FORCEINLINE const std::wstring& GetCharacterName() const { return character_name_; }
    FORCEINLINE const std::shared_ptr<SpriteRendererComponent>& GetRenderer() const { return renderer_; }
    FORCEINLINE const std::shared_ptr<AnimatorComponent>& GetAnimator() const { return animator_; }
    FORCEINLINE const std::shared_ptr<StateMachineComponent>& GetStateMachine() const { return state_machine_; }
    
    FORCEINLINE const Math::Vector2& GetVelocity() const { return velocity_; }
    FORCEINLINE void SetVelocity(const Math::Vector2& velocity) { velocity_ = velocity; }

    FORCEINLINE void SetVelocityX(float x) { velocity_.x = x; }
    FORCEINLINE float GetVelocityX() const { return velocity_.x; }
    
    FORCEINLINE void SetVelocityY(float y) { velocity_.y = y; }
    FORCEINLINE float GetVelocityY() const { return velocity_.y; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void EndPlay(EndPlayReason type) override;
    
    virtual void OnSpeakEnd();
    
#pragma region 컴포넌트
    std::shared_ptr<Controller2DComponent> controller_;
    std::shared_ptr<StateMachineComponent> state_machine_;
#pragma endregion

    std::wstring character_name_;

    Math::Vector2 velocity_;

    float gravity_;
    
#pragma region UI
    UINameTag* name_tag_;
    UIChatBalloon* chat_balloon_;
#pragma endregion

    TimerHandle chat_balloon_timer_handle_;
    
};
