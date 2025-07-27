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
    
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(const std::wstring&)>, std::decay_t<F>>>>
    void OnValueChanged(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(const std::wstring&));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnValueChanged(M* target, void(M::*func)(const std::wstring&) const);

    void OnValueChanged(void(*func)(const std::wstring&));
        
    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(void)>, std::decay_t<F>>>>
    void OnReturn(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnReturn(M* target, void(M::*func)(void));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnReturn(M* target, void(M::*func)(void) const);

    void OnReturn(void(*func)(void));
    
    FORCEINLINE const std::wstring& GetText() const { return text_->GetText(); }

    FORCEINLINE void SetPlaceholderText(const std::wstring& text) const { placeholder_text_->SetText(text); }
    FORCEINLINE const std::wstring& GetPlaceholderText() const { return placeholder_text_->GetText(); }

protected:
    virtual void Init() override;
    virtual void Tick(float delta_time) override;
    virtual void Render() override;
    
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

    Function<void(const std::wstring&)> value_changed_event_;
    Function<void(void)> return_event_;
    
};

template <typename F, typename>
void UIEditableText::OnValueChanged(F&& func)
{
    value_changed_event_ = std::forward<F>(func);
}

template <typename M, typename>
void UIEditableText::OnValueChanged(M* target, void(M::* func)(const std::wstring&))
{
    value_changed_event_ = {target, func};
}

template <typename M, typename>
void UIEditableText::OnValueChanged(M* target, void(M::* func)(const std::wstring&) const)
{
    value_changed_event_ = {target, func};
}

template <typename F, typename>
void UIEditableText::OnReturn(F&& func)
{
    return_event_ = std::forward<F>(func);
}

template <typename M, typename>
void UIEditableText::OnReturn(M* target, void(M::* func)())
{
    return_event_ = {target, func};
}

template <typename M, typename>
void UIEditableText::OnReturn(M* target, void(M::* func)() const)
{
    return_event_ = {target, func};
}
