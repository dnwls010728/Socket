#include "pch.h"
#include "GameInstanceSubsystem.h"

GameInstanceSubsystem::GameInstanceSubsystem()
{
}

void GameInstanceSubsystem::Init()
{
}

void GameInstanceSubsystem::Deinit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<GameInstanceSubsystem>("GameInstanceSubsystem")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
