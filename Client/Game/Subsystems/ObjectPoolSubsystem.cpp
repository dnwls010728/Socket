#include "pch.h"
#include "ObjectPoolSubsystem.h"

ObjectPoolSubsystem::ObjectPoolSubsystem()
{
}

void ObjectPoolSubsystem::OnWorldBeginPlay()
{
    WorldSubsystem::OnWorldBeginPlay();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<WorldSubsystem>("ObjectPoolSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
