#pragma once
#include "Inventory/Inventory.h"
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIInventory;
class Inventory;
class UIImage;

class UIInventorySlot : public UIContainer
{
    GENERATED_BODY(UIInventorySlot, UIContainer)
    
public:
    UIInventorySlot(const std::wstring& name);
    virtual ~UIInventorySlot() override = default;

    void UpdateSlot(uint32_t item_id, uint32_t count);
    void ResetSlot();

    FORCEINLINE void SetUIInventory(UIInventory* ui_inventory) { ui_inventory_ = ui_inventory; }

    FORCEINLINE void SetSlotID(uint32_t slot_id) { slot_id_ = slot_id; }
    FORCEINLINE uint32_t GetSlotID() const { return slot_id_; }

    FORCEINLINE uint32_t GetItemID() const { return item_id_; }

protected:
    virtual void Render() override;

    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;
    virtual bool OnDrop(const Math::Vector2& position, UIElement* target) override;

private:
    UIInventory* ui_inventory_;
    
    UIImage* i_icon_;
    UIText* t_count_;
    
    uint32_t slot_id_;
    uint32_t item_id_;
    
    float last_time_;
    
};
