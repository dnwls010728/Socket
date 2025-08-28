#include "pch.h"
#include "UIBuffList.h"

#include "UIBuffIcon.h"

UIBuffList::UIBuffList(const std::wstring& name) :
    UIContainer(name)
{
}

UIBuffIcon* UIBuffList::AddBuff(int32_t id, float expire_time)
{
    auto* buff_icon = AddChild<UIBuffIcon>(UIBuffIcon::StaticClass(), L"BuffIcon");
    buff_icon->Init(id, expire_time);
    return buff_icon;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIBuffList>("UIBuffList")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
