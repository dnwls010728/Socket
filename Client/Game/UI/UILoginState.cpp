#include "pch.h"
#include "UILoginState.h"

UILoginState::UILoginState()
{
}

void UILoginState::Init()
{
    UIState::Init();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UILoginState>("UILoginState")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
