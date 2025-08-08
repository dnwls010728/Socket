#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIImage;

class UIChatBalloon : public UIContainer
{
    GENERATED_BODY(UIChatBalloon, UIContainer)
    
public:
    UIChatBalloon(const std::wstring& name);
    virtual ~UIChatBalloon() override = default;

    void SetText(const std::wstring& text);

private:
    UIImage* body_;
    UIImage* tail_;

    UIText* text_;
    
};
