#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class Inventory;
class UIInventorySlot;

class UIInventory : public UIContainer
{
    GENERATED_BODY(UIInventory, UIContainer)
    
public:
    UIInventory(const std::wstring& name);
    virtual ~UIInventory() override = default;

    void UpdateSlot(uint32_t slot_index);
    void UpdateColor(uint32_t color);
    void InitInventory(Inventory* inventory);

protected:
    virtual void Init() override;
    virtual void Render() override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    std::map<uint32_t, UIInventorySlot*> slots_;

    UIText* t_color_;

    Inventory* inventory_;
    
};
