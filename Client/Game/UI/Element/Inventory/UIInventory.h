#pragma once
#include "Inventory/Inventory.h"
#include "Subsystems/Publisher/PublisherSubsystem.h"
#include "UI/UIContainer.h"
#include "UI/Element/UIImage.h"

class UIText;
class UIScrollBox;
class UIButton;
class UIInventorySlot;

class UIInventory : public UIContainer
{
    GENERATED_BODY(UIInventory, UIContainer)
    
public:
    UIInventory(const std::wstring& name);
    virtual ~UIInventory() override = default;

    void UpdateSlot(uint32_t slot_index) const;
    void UpdateColor(uint32_t color);

protected:
    virtual void Init() override;
    virtual void Uninit() override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;

private:
    friend class UIInventorySlot;
    
    void OnEvent(const EventData& event_data);

    std::array<UIButton*, static_cast<uint8_t>(Inventory::Type::kCount)> tab_buttons_;
    
    std::vector<UIInventorySlot*> slots_;

    UIImage* background_;
    UIImage* dragging_item_;

    UIScrollBox* scroll_box_;

    UIText* color_text_;

    Inventory* inventory_;

    Inventory::Type tab_;
    
};
