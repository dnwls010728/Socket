#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"
#include "UI/Element/UIImage.h"

class UISkillTooltip : public UIContainer
{
    GENERATED_BODY(UISkillTooltip, UIContainer)

public:
    UISkillTooltip(const std::wstring& name);
    virtual ~UISkillTooltip() override = default;

    void SetSkillInfo(uint32_t skill_id, int32_t level);

protected:
    virtual void Init() override;

private:
    UIImage* background_;
    UIImage* icon_slot_;
    UIImage* skill_icon_;
    UIText* name_text_;
    UIText* level_text_;
    UIText* desc_text_;
};
