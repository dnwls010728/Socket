#include "pch.h"
#include "UIPartyPanel.h"

#include "UIPartyMemberRow.h"

UIPartyPanel::UIPartyPanel(const std::wstring& name)
    : UIContainer(name), rows_()
{
    SetAbsolutePosition({ 0.f, 100.f });
    SetSize({ 200.f, 0.f });
}

void UIPartyPanel::AddOrUpdateMember(const PartyMemberInfo& info)
{
    UIPartyMemberRow* row = nullptr;
    auto it = rows_.find(info.player_id);
    if (it == rows_.end())
    {
        row = AddChild<UIPartyMemberRow>(UIPartyMemberRow::StaticClass(), L"Row" + std::to_wstring(info.player_id));
        rows_[info.player_id] = row;
    }
    else
    {
        row = it->second;
    }

    if (row) row->Update(info);
    RefreshLayout();
}

void UIPartyPanel::RemoveMember(uint32_t id)
{
    auto it = rows_.find(id);
    if (it != rows_.end())
    {
        RemoveChild(it->second);
        rows_.erase(it);
        RefreshLayout();
    }
}

void UIPartyPanel::Clear()
{
    for (auto& kv : rows_)
        RemoveChild(kv.second);
    rows_.clear();
    RefreshLayout();
}

void UIPartyPanel::RefreshLayout()
{
    float y = 0.f;
    for (auto& kv : rows_)
    {
        kv.second->SetRelativePosition({ 0.f, y });
        y += kv.second->GetSize().y;
    }
    SetSize({ GetSize().x, y });
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<UIPartyPanel>("UIPartyPanel")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
