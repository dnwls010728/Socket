#include "pch.h"
#include "GameSubsystem.h"

GameSubsystem::GameSubsystem()
{
}

void GameSubsystem::Init()
{
    GameInstanceSubsystem::Init();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<GameSubsystem>("GameSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
