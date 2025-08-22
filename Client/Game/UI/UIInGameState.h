#pragma once
#include "UI/Element/UIPopup.h"
#include "UI/UIState.h"

class UIMenu;
class UIContextMenu;
class UIChatBar;
class UIItemTooltip;
class UIInventory;

class UIInGameState : public UIState
{
    GENERATED_BODY(UIInGameState, UIState)
    
public:
    UIInGameState();
    virtual ~UIInGameState() override = default;

    FORCEINLINE UIItemTooltip* GetItemTooltip() const { return item_tooltip_; }
    FORCEINLINE UIContextMenu* GetContextMenu() const { return context_menu_; }

protected:
    virtual void Init() override;
    virtual bool OnKey(uint32_t scancode, bool is_pressed) override;

private:
    UIChatBar* char_bar_;
    UIInventory* inventory_;
    UIItemTooltip* item_tooltip_;
    UIMenu* menu_;
    UIContextMenu* context_menu_;
};
