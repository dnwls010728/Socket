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


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<TestSubsystem>("TestSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
