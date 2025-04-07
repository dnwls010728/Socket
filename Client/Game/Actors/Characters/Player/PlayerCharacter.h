#pragma once
#include "Actors/Characters/CharacterBase.h"
#include "Math/Vector2.h"

class PlayerController;

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    void OnMovement(const Math::Vector2& kDirection, float delta_time);
    void OnAttack();

protected:
    virtual void BeginPlay() override;

#pragma region 컴포넌트
    std::shared_ptr<PlayerController> controller_;
#pragma endregion
    
};
