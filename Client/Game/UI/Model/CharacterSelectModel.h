#pragma once
#include "Actors/Characters/Player/PlayerCharacter.h"

class CharacterSelectModel
{
public:
    CharacterSelectModel(const std::vector<CharacterProfile>& profiles);
    ~CharacterSelectModel() = default;

    FORCEINLINE const std::vector<CharacterProfile>& GetCharacterProfiles() const { return character_profiles; }

private:
    std::vector<CharacterProfile> character_profiles;
    
};
