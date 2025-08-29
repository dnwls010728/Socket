#include "pch.h"
#include "UIBuffList.h"

#include "UIBuffIcon.h"

UIBuffList::UIBuffList(const std::wstring& name) :
    UIContainer(name),
    buff_icons()
{
}

UIBuffIcon* UIBuffList::AddBuff(int32_t id, float expire_time)
{
    auto* buff_icon = AddChild<UIBuffIcon>(UIBuffIcon::StaticClass(), L"BuffIcon");
    buff_icon->Init(id, expire_time);
    UpdateLayout();
    return buff_icon;
}

void UIBuffList::UpdateLayout()
{
    float width = 0.f;
    for (int32_t i = 0; i < children_.size(); ++i)
    {
        auto* child = children_[i].get();
        child->SetRelativePosition({ i * 32.f , 0.f });
        
        width += child->GetSize().x;
    }

    SetSize({ width, 32.f });
    SetAbsolutePosition({1366.f - width - 10.f, 716.f});
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
