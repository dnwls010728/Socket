#pragma once
#include "UI/UIContainer.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"

#include <CommonObject.h>


class Inventory;
class UIEquipmentSlot;
class UIImage;

class UIEquipment : public UIContainer
{
    GENERATED_BODY(UIEquipment, UIContainer)
    
public:
    UIEquipment(const std::wstring& name);
    virtual ~UIEquipment() override = default;
    
    void UpdateSlot(uint32_t slot_id) const;

protected:
    virtual void Init() override;
    virtual void Uninit() override;
    
    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    void OnEvent(const EventData& data);

    std::array<UIEquipmentSlot*, static_cast<uint8_t>(EquipSlot::kCount)> slots_;
    
    UIImage* background_;
    
};
