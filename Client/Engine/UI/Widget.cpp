#include "pch.h"
#include "Widget.h"

UI::Widget::Widget() :
    parent_(nullptr),
    children_()
{
}

void UI::Widget::Tick(float delta_time)
{
}

void UI::Widget::Render()
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UI::Widget>("Widget")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
