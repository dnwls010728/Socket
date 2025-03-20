#include "pch.h"
#include "Widget.h"

#include "Input/Keyboard.h"

UI::Widget::Widget()
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

    registration::class_<UI::Widget>("UI::Widget")
        .constructor<>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
