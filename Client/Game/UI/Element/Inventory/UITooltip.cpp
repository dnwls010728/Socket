#include "pch.h"
#include "UITooltip.h"

UITooltip::UITooltip(const std::wstring& name) :
    UIContainer(name)
{
}

void UITooltip::Set(uint32_t item_id)
{
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UITooltip>("UITooltip")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
