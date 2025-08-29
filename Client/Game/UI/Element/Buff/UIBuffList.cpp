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
    auto [it, inserted] = buff_icons.try_emplace(id, nullptr);
    if (!inserted) RemoveChild(it->second);

    auto* icon = AddChild<UIBuffIcon>(UIBuffIcon::StaticClass(), L"BuffIcon_" + std::to_wstring(id));
    icon->Init(id, expire_time);

    it->second = icon;
    UpdateLayout();
    return icon;
}

void UIBuffList::Init()
{
    UIContainer::Init();
    PublisherSubsystem::Get()->Subscribe(PublisherSubsystem::EventType::kBuffExpired, this, &UIBuffList::OnEvent);
}

void UIBuffList::Uninit()
{
    UIContainer::Uninit();
    PublisherSubsystem::Get()->Unsubscribe(PublisherSubsystem::EventType::kBuffExpired, this, &UIBuffList::OnEvent);
}

void UIBuffList::OnEvent(const EventData& data)
{
    if (auto* buff_data = dynamic_cast<const BuffExpiredData*>(&data))
    {
        auto it = buff_icons.find(buff_data->buff_id);
        if (it != buff_icons.end())
        {
            buff_icons.erase(it);
            UpdateLayout();
        }
    }
}

void UIBuffList::UpdateLayout()
{
    std::vector<UIElement*> children;
    GetChildren(children);
    
    float width = 0.f;
    for (int32_t i = 0; i < children.size(); ++i)
    {
        auto* child = children[i];
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
