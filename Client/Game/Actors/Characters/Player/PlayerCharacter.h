#pragma once
#include "Actors/Characters/CharacterBase.h"

class PlayerCharacter : public CharacterBase
{
    SHADER_CLASS_HELPER(PlayerCharacter)
    GENERATED_BODY(PlayerCharacter, CharacterBase)
    
public:
    PlayerCharacter(const std::wstring& kName);
    virtual ~PlayerCharacter() override = default;

    FORCEINLINE bool IsOwner() const { return is_owner_; }
    FORCEINLINE void SetOwner(bool bIsOwner) { is_owner_ = bIsOwner; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;

    bool is_owner_;
    
};
