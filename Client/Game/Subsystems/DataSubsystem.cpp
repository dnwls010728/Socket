#include "pch.h"
#include "DataSubsystem.h"

DataSubsystem::DataSubsystem()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<DataSubsystem>("DataSubsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
