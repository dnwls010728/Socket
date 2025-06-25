#include "pch.h"
#include "WorldSubsystem.h"

WorldSubsystem::WorldSubsystem()
{
}

void WorldSubsystem::OnWorldBeginPlay()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<WorldSubsystem>("WorldSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
