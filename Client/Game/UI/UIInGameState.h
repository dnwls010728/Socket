#pragma once
#include "UI/UIState.h"

class UIInventory;

class UIInGameState : public UIState
{
    GENERATED_BODY(UIInGameState, UIState)
    
public:
    UIInGameState();
    virtual ~UIInGameState() override = default;

protected:
    virtual void Init() override;

    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;

private:
    UIInventory* inventory_;
    
};
