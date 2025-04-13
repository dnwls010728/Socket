#include "pch.h"
#include "TestSubsystem.h"

TestSubsystem::TestSubsystem()
{
}


RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<TestSubsystem>("TestSubsystem")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
