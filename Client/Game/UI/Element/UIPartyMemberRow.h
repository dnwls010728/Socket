#pragma once
#include "UI/UIContainer.h"
#include <CommonObject.h>
#include "UI/Element/UIText.h"

class UIPartyMemberRow : public UIContainer
{
    GENERATED_BODY(UIPartyMemberRow, UIContainer)

public:
    UIPartyMemberRow(const std::wstring& name);
    virtual ~UIPartyMemberRow() override = default;

    void Update(const PartyMemberInfo& info);

protected:
    virtual void Render() override;

private:
    UIText* name_text_;
    UIText* hp_text_;
    PartyMemberInfo info_;
};
