#pragma once
#include "UI/UIContainer.h"

class UIImage;

class UICharacterCreate : public UIContainer
{
    GENERATED_BODY(UICharacterCreate, UIContainer)
    
public:
    UICharacterCreate(const std::wstring& name);
    virtual ~UICharacterCreate() override = default;

protected:
    virtual void Init() override;

private:
    UIImage* background_;
};
