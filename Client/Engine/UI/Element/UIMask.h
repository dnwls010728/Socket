#pragma once
#include "UI/UIContainer.h"

class UIMask : public UIContainer
{
    GENERATED_BODY(UIMask, UIContainer)
    
public:
    UIMask(const std::wstring& name);
    virtual ~UIMask() override = default;

protected:
    virtual void Render() override;
    
};
