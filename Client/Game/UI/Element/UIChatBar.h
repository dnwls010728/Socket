#pragma once
#include "UI/UIContainer.h"

class UIEditableText;
class UIImage;

class UIChatBar : public UIContainer
{
    GENERATED_BODY(UIChatBar, UIContainer)
    
public:
    UIChatBar(const std::wstring& name);
    virtual ~UIChatBar() override = default;

private:
    UIImage* input_background_;
    UIImage* chat_background_;

    UIEditableText* input_text_;
    
};
