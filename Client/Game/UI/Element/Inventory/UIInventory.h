#pragma once
#include "UI/UIContainer.h"

class UIInventorySlot;

class UIInventory : public UIContainer
{
    GENERATED_BODY(UIInventory, UIContainer)
    
public:
    UIInventory();
    virtual ~UIInventory() override = default;

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    std::vector<UIInventorySlot*> slots_;
    
};
