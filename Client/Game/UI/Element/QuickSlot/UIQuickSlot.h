#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIImage;

class UIQuickSlot : public UIContainer
{
    GENERATED_BODY(UIQuickSlot, UIContainer)
    
public:
    UIQuickSlot(const std::wstring& name);
    virtual ~UIQuickSlot() override = default;

private:
    UIImage* background_;
    UIImage* icon_;

    UIText* key_name_text_;
    UIText* count_text_;
    
};
