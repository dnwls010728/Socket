#pragma once
#include "UI/UIContainer.h"

class Inventory;
class UIInventorySlot;

class UIInventory : public UIContainer
{
    GENERATED_BODY(UIInventory, UIContainer)
    
public:
    UIInventory(const std::wstring& name);
    virtual ~UIInventory() override = default;

protected:
    virtual void Init() override;
    virtual void Render() override;

    virtual bool OnDragBegin(const Math::Vector2& position) override;
    virtual bool OnDrag(const Math::Vector2& position, const Math::Vector2& delta) override;
    virtual bool OnDragEnd(const Math::Vector2& position) override;

private:
    std::map<uint32_t, UIInventorySlot*> slots_;
    
};
