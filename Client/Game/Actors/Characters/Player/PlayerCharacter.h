#pragma once
#include "Actors/Characters/CharacterBase.h"

namespace UI
{
    class TextBox;
}

class PlayerController;

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    void OnAttack();
    void OnMovement();
    void ClearLinearVelocity();

protected:
    virtual void BeginPlay() override;

#pragma region 컴포넌트
    std::shared_ptr<PlayerController> controller_;
#pragma endregion

    float move_speed_;

    std::shared_ptr<UI::TextBox> nickname_text_box_;

    TimerHandle timer_handle_;
    
};
