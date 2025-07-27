#pragma once
#include "UI/UIContainer.h"

class UIText;
class UIScrollBox;
class UIEditableText;
class UIImage;

class UIChatBar : public UIContainer
{
    GENERATED_BODY(UIChatBar, UIContainer)
    
public:
    UIChatBar(const std::wstring& name);
    virtual ~UIChatBar() override = default;

    void FocusInput();

private:
    void OnReturn();
    
    UIImage* chat_background_;
    UIImage* input_background_;
    UIImage* target_background_;

    UIScrollBox* scroll_box_;

    UIText* target_text_;

    UIEditableText* input_text_;
    
};
