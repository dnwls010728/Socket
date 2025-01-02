#include "pch.h"
#include "UIWidget.h"

UIWidget::UIWidget()
{
}

void UIWidget::Render()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIWidget>("UIWidget")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
