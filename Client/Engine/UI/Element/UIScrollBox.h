#pragma once
#include "UI/UIContainer.h"

class UIScrollBox : public UIContainer
{
    GENERATED_BODY(UIScrollBox, UIContainer)
    
public:
    UIScrollBox(const std::wstring& name);
    virtual ~UIScrollBox() override = default;

    void SetScrollY(float scroll_y);

protected:
    virtual void Render() override;

    float scroll_x_ = 0.f;
    float scroll_y_ = 0.f;
    
};
