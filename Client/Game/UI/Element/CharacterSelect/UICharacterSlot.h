#pragma once
#include "UI/UIContainer.h"
#include "UI/Model/CharacterSelectModel.h"

class UIText;
class UIImage;

class UICharacterSlot : public UIContainer
{
    GENERATED_BODY(UICharacterSlot, UIContainer)
    
public:
    UICharacterSlot(const std::wstring& name);
    virtual ~UICharacterSlot() override = default;

    void InitSlot(const CharacterProfile& profile);

protected:
    virtual void Init() override;

private:
    UIImage* background_;

    UIText* empty_text_;
    
};
