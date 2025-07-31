#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIImage.h"
#include "UI/Model/CharacterSelectModel.h"

class UICharacterSlot;

class UICharacterSelect : public UIContainer
{
    GENERATED_BODY(UICharacterSelect, UIContainer)
    
public:
    UICharacterSelect(const std::wstring& name);
    virtual ~UICharacterSelect() override = default;

    void InitSlots(const std::vector<CharacterProfile>& profiles) const;

protected:
    virtual void Init() override;

private:
    UIImage* background_;
    
    std::vector<UICharacterSlot*> slots_;
};
