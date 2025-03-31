#pragma once
#include "Actors/Characters/CharacterBase.h"
#include "Math/Vector2.h"

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    void SpawnBomb();
    void OnMovement(const Math::Vector2& kMovement, float delta_time);

protected:
    virtual void BeginPlay() override;
    
#pragma region COMPONENTS
    class PlayerController* controller_;
#pragma endregion
    
};
