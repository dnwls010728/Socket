#pragma once
#include "UIMask.h"
#include "UIText.h"
#include "Misc/FunctionMacros.h"

class UIMask;

class UIEditableText : public UIMask
{
    GENERATED_BODY(UIEditableText, UIMask)
    
public:
    enum class ContentType : uint8_t
    {
        kStandard,
        kIntegerNumber,
        kDecimalNumber,
        kAlphanumeric,
        kPassword
    };
    
    UIEditableText(const std::wstring& name);
    virtual ~UIEditableText() override = default;

    virtual void SetSize(const Math::Vector2& size) override;

    void SetText(const std::wstring& text);
    void SetContentType(ContentType type);
    
    FORCEINLINE const std::wstring& GetText() const { return text_buffer_; }

    FORCEINLINE void SetPlaceholderText(const std::wstring& text) const { placeholder_text_->SetText(text); }
    FORCEINLINE const std::wstring& GetPlaceholderText() const { return placeholder_text_->GetText(); }

    FORCEINLINE void SetCharacterLimit(int32_t limit) { character_limit_ = limit; }
    FORCEINLINE int32_t GetCharacterLimit() const { return character_limit_; }

    FORCEINLINE ContentType GetContentType() const { return content_type_; }

    DEFINE_BIND_OVERLOADS(value_changed_event_, OnValueChanged, void, const std::wstring&)
    DEFINE_BIND_OVERLOADS(return_event_, OnReturn, void, void)

protected:
    virtual void Init() override;
    virtual void Tick(float delta_time) override;
    virtual void Render() override;
    
    virtual bool OnKey(uint32_t scancode, bool is_pressed) override;
    virtual bool OnChar(wchar_t character) override;

    virtual void OnFocus(bool is_focused) override;
    
private:
    void PostCursorMove();
    void UpdateDisplayedText() const;
    void PostTextChange(bool is_reset);
    void UpdatePlaceholder() const;
    void ResetCursor();
    void ScrollToCursor();
    
    UIText* text_;
    UIText* placeholder_text_;

    int32_t cursor_position_;
    int32_t character_limit_;

    float timer_;
    float cursor_advance_;
    float text_offset_;

    bool cursor_visible_;

    std::wstring text_buffer_;

    ContentType content_type_;

    Function<void(const std::wstring&)> value_changed_event_;
    Function<void(void)> return_event_;
    
};
