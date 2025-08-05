#pragma once
#include "UI/UIState.h"

class UICharacterSelect;
class UILogin;
class UIInventory;

class UILoginState : public UIState
{
    GENERATED_BODY(UILoginState, UIState)
    
public:
    UILoginState();
    virtual ~UILoginState() override = default;

protected:
    virtual void Init() override;

private:
    void OnExit();
    
};
