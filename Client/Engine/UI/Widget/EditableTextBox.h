#pragma once
#include "UI/Widget.h"

DECLARE_DELEGATE(OnTextEvent, wchar_t)

enum class ContentType
{
    Standard,
    Password
};

class EditableTextBox : public Widget
{
    SHADER_CLASS_HELPER(EditableTextBox)
    GENERATED_BODY(EditableTextBox, Widget)
    
public:
    EditableTextBox(const std::wstring& kName);
    virtual ~EditableTextBox() override = default;

    // FORCEINLINE void SetText(const std::wstring& kText) { text_ = kText; }
    FORCEINLINE const std::wstring& GetText() const { return text_; }

    FORCEINLINE void SetPlaceholder(const std::wstring& kPlaceholder) { placeholder_ = kPlaceholder; }
    FORCEINLINE void SetContentType(ContentType content_type) { content_type_ = content_type; }

    OnTextEvent OnTextChanged;

protected:
    virtual void Tick(float delta_time) override;
    virtual void Render() override;
    virtual void UpdateRect() override;
    virtual void OnFocusChanged(bool is_focused) override;
    virtual void OnInputKey(Type::uint16 key_code, bool is_pressed) override;
    virtual void OnInputText(wchar_t character) override;

private:
    friend class Canvas;
    
    std::wstring text_;
    std::wstring placeholder_;

    Math::Rect text_rect_;

    int cursor_index_;

    float elapsed_time_;

    bool cursor_visible_;

    std::vector<float> advances_;

    ContentType content_type_;
    
};
