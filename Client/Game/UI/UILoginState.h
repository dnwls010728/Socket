#pragma once
#include "Model/CharacterSelectModel.h"
#include "UI/UIState.h"
#include "UI/Element/UIButton.h"

class UICharacterSelect;
class UILogin;
class UIInventory;

class UILoginState : public UIState
{
    GENERATED_BODY(UILoginState, UIState)
    
public:
    UILoginState();
    virtual ~UILoginState() override = default;
    
    void InitModel(const std::vector<CharacterProfile>& profiles);

    FORCEINLINE UILogin* GetLogin() const { return login_; }
    FORCEINLINE UICharacterSelect* GetCharacterSelect() const { return character_select_; }

protected:
    virtual void Init() override;

private:
    void OnExit();

    UILogin* login_;
    UICharacterSelect* character_select_;

    std::unique_ptr<CharacterSelectModel> character_select_model_;
    
};
