#pragma once
#include "UIMask.h"
#include "UIText.h"

class UIMask;

class UIEditableText : public UIMask
{
    GENERATED_BODY(UIEditableText, UIMask)
    
public:
    UIEditableText(const std::wstring& name);
    virtual ~UIEditableText() override = default;

    void SetText(const std::wstring& text);
    
    FORCEINLINE const std::wstring& GetText() const { return text_->GetText(); }

    FORCEINLINE void SetPlaceholderText(const std::wstring& text) const { placeholder_text_->SetText(text); }
    FORCEINLINE const std::wstring& GetPlaceholderText() const { return placeholder_text_->GetText(); }

protected:
    virtual void Init() override;
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;
    
    virtual bool OnKey(uint16_t key_code, bool is_pressed) override;
    virtual bool OnChar(wchar_t character) override;

    virtual void OnFocus(bool is_focused) override;
    
private:
    void PostCursorMove();
    void PostTextChange(bool is_reset);
    void UpdatePlaceholder() const;
    void ResetCursor();
    void ScrollToCursor();
    
    UIText* text_;
    UIText* placeholder_text_;

    int32_t cursor_position_;

    float timer_;
    float cursor_advance_;
    float text_offset_;

    bool cursor_visible_;
    
};
