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
    virtual void Tick(float delta_time) override;
private:
    UIText* name_text_;
    UIText* hp_text_;
    PartyMemberInfo info_;

    float timer_;
    float hp_effect_ratio_;
};
