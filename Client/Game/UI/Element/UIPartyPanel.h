#pragma once
#include "UI/UIContainer.h"
#include <CommonObject.h>
#include <unordered_map>

class UIPartyMemberRow;

class UIPartyPanel : public UIContainer
{
    GENERATED_BODY(UIPartyPanel, UIContainer)

public:
    UIPartyPanel(const std::wstring& name);
    virtual ~UIPartyPanel() override = default;

    void AddOrUpdateMember(const PartyMemberInfo& info);
    void RemoveMember(uint32_t id);
    void Clear();

private:
    void RefreshLayout();

    std::unordered_map<uint32_t, UIPartyMemberRow*> rows_;
};
