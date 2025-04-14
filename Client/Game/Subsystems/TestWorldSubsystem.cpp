#include "pch.h"
#include "TestWorldSubsystem.h"

TestWorldSubsystem::TestWorldSubsystem()
{
}

void TestWorldSubsystem::Init()
{
    WorldSubsystem::Init();

    Logger::Print(L"TestWorldSubsystem initialized.");
}

void TestWorldSubsystem::Deinit()
{
    WorldSubsystem::Deinit();

    Logger::Print(L"TestWorldSubsystem deinitialized.");
}

void TestWorldSubsystem::Tick(float delta_time)
{
    Tickable::Tick(delta_time);

    // Logger::Print(L"TestWorldSubsystem Tick: %f", delta_time);
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<TestWorldSubsystem>("TestWorldSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
