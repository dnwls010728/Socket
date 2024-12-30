#include "pch.h"
#include "UIElement.h"

UIElement::UIElement()
{
}

void UIElement::Render()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIElement>("UIElement")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
