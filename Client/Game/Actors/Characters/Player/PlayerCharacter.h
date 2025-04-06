#pragma once
#include "Actors/Characters/CharacterBase.h"

class PlayerController;

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

protected:
    virtual void BeginPlay() override;

#pragma region 컴포넌트
    std::shared_ptr<PlayerController> controller_;
#pragma endregion
    
};
