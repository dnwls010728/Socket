#include "pch.h"
#include "Subsystem.h"

Subsystem::Subsystem()
{
}

void Subsystem::Init()
{
}

void Subsystem::Deinit()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Subsystem>("Subsystem")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
