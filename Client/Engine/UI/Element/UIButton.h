#pragma once
#include "Math/Color.h"
#include "UI/UIContainer.h"

class UIImage;
class UIText;

class UIButton : public UIContainer
{
    GENERATED_BODY(UIButton, UIContainer)
    
public:
    enum class State : uint8_t
    {
        kNormal = 0,
        kHover,
        kPressed,
        kDisabled,
        kCount
    };
    
    UIButton(const std::wstring& name);
    virtual ~UIButton() override = default;

    void SetDisabled(bool is_disabled);
    void SetSprite(State state, UISprite* sprite, const std::wstring& frame_name);

    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<void(void)>, std::decay_t<F>>>>
    void OnClick(F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnClick(M* target, void(M::*func)(void));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void OnClick(M* target, void(M::*func)(void) const);

    void OnClick(void(*func)(void));
    
    void SetText(const std::wstring& text) const;
    void SetTextColor(const Math::Color& color) const;

protected:
    void ChangeState(State state);
    
    virtual void Init() override;

    virtual UI::MouseEventResult OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;

    virtual bool OnMouseEnter() override;
    virtual bool OnMouseLeave() override;

    UIImage* ui_image_;
    UIText* ui_text_;
    
    std::array<std::pair<UISprite*, std::wstring>, static_cast<uint8_t>(State::kCount)> sprites_;

    State current_state_;

    Function<void(void)> click_event_;
    
};

template <typename F, typename>
void UIButton::OnClick(F&& func)
{
    click_event_ = std::forward<F>(func);
}

template <typename M, typename>
void UIButton::OnClick(M* target, void(M::* func)())
{
    click_event_ = {target, func};
}

template <typename M, typename>
void UIButton::OnClick(M* target, void(M::* func)() const)
{
    click_event_ = {target, func};
}
