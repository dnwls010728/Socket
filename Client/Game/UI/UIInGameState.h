#pragma once
#include "UI/UIPopup.h"
#include "UI/UIState.h"

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

    void ShowPopup(std::wstring caption, PopupOption option, std::function<bool(std::wstring, PopupOption)> callback);
protected:
    virtual void Init() override;
    virtual void Tick(float deltaTime) override;
    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;

private:
    void PostTask(std::function<void()> task);
    
    UIChatBar* char_bar_;
    UIInventory* inventory_;
    UIItemTooltip* item_tooltip_;
    
    std::queue<std::function<void()>> pending_tasks_;
};
