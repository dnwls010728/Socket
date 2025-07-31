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

    void InitSlot(const CharacterProfile& profile) const;

protected:
    virtual void Init() override;

private:
    UIImage* background_;
    UIImage* character_;

    UIText* empty_text_;
    UIText* lv_text_;
    UIText* name_text_;
    UIText* color_code_text_;
    
};
