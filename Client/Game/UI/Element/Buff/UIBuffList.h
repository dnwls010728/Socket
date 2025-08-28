#pragma once
#include "UI/UIContainer.h"

class UIBuffIcon;

class UIBuffList : public UIContainer
{
    GENERATED_BODY(UIBuffList, UIContainer)
    
public:
    UIBuffList(const std::wstring& name);
    virtual ~UIBuffList() override = default;

    UIBuffIcon* AddBuff(int32_t id, float expire_time);
    
};
