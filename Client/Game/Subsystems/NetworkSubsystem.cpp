#include "pch.h"
#include "NetworkSubsystem.h"

NetworkSubsystem::NetworkSubsystem()
{
}

void NetworkSubsystem::Init()
{
    WorldSubsystem::Init();

    Logger::Print(L"TestWorldSubsystem initialized.");
}

void NetworkSubsystem::Deinit()
{
    WorldSubsystem::Deinit();

    Logger::Print(L"TestWorldSubsystem deinitialized.");
}

void NetworkSubsystem::Tick(float delta_time)
{
    Tickable::Tick(delta_time);

    // Logger::Print(L"TestWorldSubsystem Tick: %f", delta_time);
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<NetworkSubsystem>("NetworkSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
