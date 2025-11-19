#pragma once
#include "UITooltip.h"
#include "PacketHandlers/PlayerStatsUpdateHandler.h"

class UIImage;
class UIText;

class UIEquipTooltip : public UITooltip
{
    GENERATED_BODY(UIEquipTooltip, UITooltip)
    
public:
    UIEquipTooltip(const std::wstring& name);
    virtual ~UIEquipTooltip() override = default;
    
    virtual void Set(uint32_t item_id) override;
    
protected:
    virtual void Init() override;
    
    UIImage* frame_;
    UIImage* icon_slot_;
    UIImage* item_icon_;

    UIText* item_name_;
    UIText* req_lv_;
    UIText* max_hp_;
    UIText* atk_;
    UIText* def_;
    UIText* dig_;
    
};
