#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIImage;

class UIItemTooltip : public UIContainer
{
    GENERATED_BODY(UIItemTooltip, UIContainer)
    
public:
    UIItemTooltip(const std::wstring& name);
    virtual ~UIItemTooltip() override = default;

    void Set(uint32_t item_id);

protected:
    virtual void Init() override;
    
    UIImage* frame_;
    UIImage* icon_slot_;
    UIImage* item_icon_;

    UIText* item_name_;
    UIText* item_description_;
    
};
