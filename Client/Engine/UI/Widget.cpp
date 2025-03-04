#include "pch.h"
#include "Widget.h"

UI::Widget::Widget()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::Widget>("UI::Widget")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
