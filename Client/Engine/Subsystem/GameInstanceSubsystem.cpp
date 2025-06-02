#include "pch.h"
#include "GameInstanceSubsystem.h"

GameInstanceSubsystem::GameInstanceSubsystem()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<GameInstanceSubsystem>("GameInstanceSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
