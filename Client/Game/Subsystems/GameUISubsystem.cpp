#include "pch.h"
#include "GameUISubsystem.h"

GameUISubsystem::GameUISubsystem()
{
}

void GameUISubsystem::Init()
{
    GameInstanceSubsystem::Init();
}

void GameUISubsystem::Deinit()
{
    GameInstanceSubsystem::Deinit();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<GameUISubsystem>("GameUISubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
