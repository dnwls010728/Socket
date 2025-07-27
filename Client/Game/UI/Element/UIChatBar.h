#pragma once
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIEditableText;
class UIImage;

class UIChatBar : public UIContainer
{
    GENERATED_BODY(UIChatBar, UIContainer)
    
public:
    UIChatBar(const std::wstring& name);
    virtual ~UIChatBar() override = default;

private:
    UIImage* chat_background_;
    UIImage* input_background_;
    UIImage* target_background_;

    UIText* target_text_;

    UIEditableText* input_text_;
    
};
