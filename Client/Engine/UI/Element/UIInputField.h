#pragma once
#include "UI/UIContainer.h"

class UIInputField : public UIContainer
{
    GENERATED_BODY(UIInputField, UIContainer)
    
public:
    UIInputField(const std::wstring& name);
    virtual ~UIInputField() override = default;
    
};
