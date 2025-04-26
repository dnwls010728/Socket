#pragma once
#include <CommonObject.h>

#include "Actors/Characters/CharacterBase.h"

namespace UI
{
    class ChatBalloon;
}

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    virtual void ReceivePacket(Net::IPacket* packet) override;

    void InitSpawn(const Math::Vector2& position);
    void Speak(const std::wstring& message);

    FORCEINLINE Math::Vector2& GetVelocity() { return velocity_; }

protected:
    virtual void BeginPlay() override;
    virtual void PhysicsTick(float delta_time) override;
    virtual void Tick(float delta_time) override;
    virtual void EndPlay(EndPlayReason type) override;

    Math::Vector2 movement_input_;

    Movement last_movement_;
    std::queue<Movement> movements_;

    bool is_jump_;

    int timer_;

#pragma region UI
    std::shared_ptr<UI::ChatBalloon> chat_balloon_;
#pragma endregion

    TimerHandle chat_balloon_timer_handle_;
    
};
