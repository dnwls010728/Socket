#pragma once
#include "UI/Widget.h"

DECLARE_DELEGATE(OnTextEvent, wchar_t)

class EditableTextBox : public Widget
{
    SHADER_CLASS_HELPER(EditableTextBox)
    GENERATED_BODY(EditableTextBox, Widget)
    
public:
    EditableTextBox(const std::wstring& kName);
    virtual ~EditableTextBox() override = default;

    OnTextEvent OnTextChanged;

protected:
    virtual void Tick(float delta_time) override;
    virtual void Render() override;
    virtual void OnInputKey(Type::uint16 key_code, bool is_pressed) override;
    virtual void OnInputText(wchar_t character) override;

private:
    friend class Canvas;
    
    std::wstring text_;

    int cursor_position_;

    float elapsed_time_;

    bool cursor_visible_;

    std::vector<float> advances_;
    
};
