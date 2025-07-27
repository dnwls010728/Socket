#include "pch.h"
#include "UIScrollBox.h"

UIScrollBox::UIScrollBox(const std::wstring& name) :
    UIMask(name)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIScrollBox>("UIScrollBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
