#include "pch.h"
#include "TestSubsystem.h"

TestSubsystem::TestSubsystem()
{
}

void TestSubsystem::Init()
{
    GameInstanceSubsystem::Init();

    Logger::Print(L"TestSubsystem initialized.");
}

void TestSubsystem::Deinit()
{
    GameInstanceSubsystem::Deinit();

    Logger::Print(L"TestSubsystem deinitialized.");
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<TestSubsystem>("TestSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
