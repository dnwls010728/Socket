#pragma once
#include "UI/UIContainer.h"

class UITooltip : public UIContainer
{
    GENERATED_BODY(UITooltip, UIContainer)
    
public:
    UITooltip(const std::wstring& name);
    virtual ~UITooltip() override = default;
    
    virtual void Set(uint32_t item_id);
    
};
