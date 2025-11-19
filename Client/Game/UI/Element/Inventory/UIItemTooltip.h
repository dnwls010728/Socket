#pragma once
#include "UITooltip.h"

class UIImage;
class UIText;

class UIItemTooltip : public UITooltip
{
    GENERATED_BODY(UIItemTooltip, UITooltip)
    
public:
    UIItemTooltip(const std::wstring& name);
    virtual ~UIItemTooltip() override = default;

    virtual void Set(uint32_t item_id) override;

protected:
    virtual void Init() override;
    
    UIImage* frame_;
    UIImage* icon_slot_;
    UIImage* item_icon_;

    UIText* item_name_;
    UIText* item_description_;
    
};
