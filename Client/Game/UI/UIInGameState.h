#pragma once
#include "UI/UIState.h"

class UIItemTooltip;
class UIInventory;

class UIInGameState : public UIState
{
    GENERATED_BODY(UIInGameState, UIState)
    
public:
    UIInGameState();
    virtual ~UIInGameState() override = default;

    FORCEINLINE UIItemTooltip* GetItemTooltip() const { return item_tooltip_; }

protected:
    virtual void Init() override;

    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;

private:
    UIInventory* inventory_;

    UIItemTooltip* item_tooltip_;
    
};
