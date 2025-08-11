#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIImage;

class UIMenu : public UIContainer
{
    GENERATED_BODY(UIMenu, UIContainer)
    
public:
    UIMenu(const std::wstring& name);
    virtual ~UIMenu() override = default;

protected:
    virtual void Init() override;

private:
    UIImage* background_;

    UIText* title_text_;
    
};
