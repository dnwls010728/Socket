#pragma once
#include "UI_OLD/Widget_OLD.h"

DECLARE_DELEGATE(OnTextEvent, wchar_t)

enum class ContentType
{
    Standard,
    Password
};

class EditableTextBox : public Widget_OLD
{
    SHADER_CLASS_HELPER(EditableTextBox)
    GENERATED_BODY(EditableTextBox, Widget_OLD)
    
public:
    EditableTextBox(const std::wstring& kName);
    virtual ~EditableTextBox() override = default;

    void SetText(const std::wstring& kText);

    FORCEINLINE const std::wstring& GetText() const { return text_; }

    FORCEINLINE void SetFontFamily(const std::wstring& kFontFamily) { font_family_ = kFontFamily; }
    FORCEINLINE const std::wstring& GetFontFamily() const { return font_family_; }

    FORCEINLINE void SetPlaceholder(const std::wstring& kPlaceholder) { placeholder_ = kPlaceholder; }
    FORCEINLINE void SetContentType(ContentType content_type) { content_type_ = content_type; }

    OnTextEvent OnTextChanged;

protected:
    virtual void Tick(float delta_time) override;
    virtual void Render() override;
    virtual void UpdateRect() override;

    virtual bool OnFocus(bool is_focused) override;
    virtual bool OnKey(Type::uint16 key_code, bool is_pressed) override;
    virtual bool OnChar(wchar_t character) override;

private:
    friend class Canvas_OLD;

    void UpdateAdvances(const std::wstring& kString);
    
    std::wstring text_;
    std::wstring placeholder_;
    std::wstring font_family_;

    Math::Rect text_rect_;

    int cursor_index_;

    float elapsed_time_;

    bool cursor_visible_;

    std::vector<float> advances_;

    ContentType content_type_;
    
};
