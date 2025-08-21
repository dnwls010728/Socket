#pragma once
#include "UIImage.h"
#include "Math/Color.h"
#include "Misc/FunctionMacros.h"
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

    virtual  void SetSize(const Math::Vector2& size) override;

    void SetDisabled(bool is_disabled);
    void SetSprite(State state, UISprite* sprite, const std::wstring& frame_name);
    void SetDrawMode(UIImage::DrawMode draw_mode) const;
    
    void SetText(const std::wstring& text) const;
    void SetTextColor(const Math::Color& color) const;

    FORCEINLINE bool IsDisabled() const { return current_state_ == State::kDisabled; }

    DEFINE_BIND_OVERLOADS(click_event_, OnClick, void, void)

protected:
    void ChangeState(State state);
    
    virtual void Init() override;

    virtual bool OnMouseButton(const Math::Vector2& position, MouseButton button, bool is_pressed, double timestamp) override;
    virtual bool OnMouseEnter() override;
    virtual bool OnMouseLeave() override;

    UIImage* ui_image_;
    UIText* ui_text_;
    
    std::array<std::pair<UISprite*, std::wstring>, static_cast<uint8_t>(State::kCount)> sprites_;

    State current_state_;

    Function<void(void)> click_event_;
    
};
