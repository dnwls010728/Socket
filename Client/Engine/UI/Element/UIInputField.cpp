#include "pch.h"
#include "UIInputField.h"

UIInputField::UIInputField(const std::wstring& name) :
    UIContainer(name)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIInputField>("UIInputField")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
