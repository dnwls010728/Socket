#include "pch.h"
#include "UIState.h"

UIState::UIState()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIState>("UIState")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
