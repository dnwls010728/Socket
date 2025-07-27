#pragma once
#include "UIMask.h"

class UIScrollBox : UIMask
{
    GENERATED_BODY(UIScrollBox, UIMask)
    
public:
    UIScrollBox(const std::wstring& name);
    virtual ~UIScrollBox() override = default;
    
};
