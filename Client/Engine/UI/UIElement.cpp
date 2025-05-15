#include "pch.h"
#include "UIElement.h"

UIElement::UIElement()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIElement>("UIElement")
        .constructor<>()
        (
            policy::ctor::as_raw_ptr
        );
}
