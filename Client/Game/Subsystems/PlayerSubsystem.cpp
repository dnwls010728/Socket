#include "pch.h"
#include "PlayerSubsystem.h"

#include "GameInstance.h"

PlayerSubsystem::PlayerSubsystem() :
    account_id_(0),
    character_id_(0),
    name_(L""),
    position_x_(0.f),
    position_y_(0.f),
    inventory_(nullptr)
{
}

PlayerSubsystem* PlayerSubsystem::Get()
{
    return GameInstance::Get()->GetSubsystem<PlayerSubsystem>();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<PlayerSubsystem>("PlayerSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
