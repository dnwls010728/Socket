#include "pch.h"
#include "UIPartyMemberRow.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIPartyMemberRow::UIPartyMemberRow(const std::wstring& name)
    : UIContainer(name),
      name_text_(nullptr),
      hp_text_(nullptr),
      info_()
{
    SetSize({ 200.f, 40.f });

    name_text_ = AddChild<UIText>(UIText::StaticClass(), L"NameText");
    name_text_->SetRelativePosition({ 4.f, 2.f });
    name_text_->SetSize({ 192.f, 16.f });
    name_text_->SetColor(Math::Color::White);

    hp_text_ = AddChild<UIText>(UIText::StaticClass(), L"HPText");
    hp_text_->SetRelativePosition({ 4.f, 20.f });
    hp_text_->SetSize({ 192.f, 16.f });
    hp_text_->SetColor(Math::Color::White);
}

void UIPartyMemberRow::Update(const PartyMemberInfo& info)
{
    info_ = info;
    name_text_->SetText(info_.name + L" Lv." + std::to_wstring(info_.lv));
    hp_text_->SetText(std::to_wstring(info_.hp) + L" / " + std::to_wstring(info_.max_hp));
}

void UIPartyMemberRow::Render()
{
    Renderer* renderer = Renderer::Get();
    float ratio = 0.f;
    if (info_.max_hp > 0)
        ratio = static_cast<float>(info_.hp) / static_cast<float>(info_.max_hp);

    Math::Vector2 pos = GetAbsolutePosition();
    renderer->DrawSolidBox(pos + Math::Vector2{0.f, 20.f}, { GetSize().x, 18.f }, Math::Color::Gray);
    renderer->DrawSolidBox(pos + Math::Vector2{0.f, 20.f}, { GetSize().x * ratio, 18.f }, Math::Color::Red);

    UIContainer::Render();
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UIPartyMemberRow>("UIPartyMemberRow")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
