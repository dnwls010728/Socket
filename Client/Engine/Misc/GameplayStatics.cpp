#include "pch.h"
#include "GameplayStatics.h"

#include "Character/CharacterBase.h"

float GameplayStatics::ApplyDamage(CharacterBase* damaged_character, float base_damage, Actor* event_instigator, Actor* damage_causer)
{
    if (IsValid(damaged_character) && base_damage != 0.f)
    {
        return damaged_character->TakeDamage(base_damage, event_instigator, damage_causer);
    }

    return 0.f;
}
