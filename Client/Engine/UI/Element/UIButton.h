#pragma once
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
    
    FORCEINLINE UIText* GetText() const { return ui_text_; }

protected:
    void ChangeState(State state);
    
    virtual void Init() override;

    UIImage* ui_image_;
    UIText* ui_text_;
    
    std::array<std::pair<UISprite*, std::wstring>, static_cast<uint8_t>(State::kCount)> sprites_;

    State current_state_;
    
};
