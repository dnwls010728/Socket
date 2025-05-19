#pragma once
#include "UI/UIState.h"

class UIInventory;

class UILoginState : public UIState
{
    GENERATED_BODY(UILoginState, UIState)
    
public:
    UILoginState();
    virtual ~UILoginState() override = default;

    FORCEINLINE UIInventory* GetInventory() const { return inventory_; }

protected:
    virtual void Init() override;

private:
    UIInventory* inventory_;
    
};
