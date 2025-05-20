#include "pch.h"
#include "PlayerSubsystem.h"

#include "GameInstance.h"

PlayerSubsystem::PlayerSubsystem() :
    inventory_(nullptr)
{
}

PlayerSubsystem* PlayerSubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<PlayerSubsystem>();
}
