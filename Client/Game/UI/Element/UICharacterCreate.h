#pragma once
#include "Math/Color.h"
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIColorPicker;
class UIButton;
class UIEditableText;
class UIImage;

class UICharacterCreate : public UIContainer
{
    GENERATED_BODY(UICharacterCreate, UIContainer)
    
public:
    UICharacterCreate(const std::wstring& name);
    virtual ~UICharacterCreate() override = default;

    void OnCheckResult(bool is_available) const;

protected:
    virtual void Init() override;
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

private:
    void OnColorChanged(const Math::Color& color) const;
    void OnClick() const;
    void OnConfirm() const;
    void OnCancel();
    
    UIImage* background_;
    UIImage* character_;
    UIImage* name_background_;

    UIText* title_text_;
    UIText* color_text_;

    UIColorPicker* color_picker_;

    UIEditableText* name_input_;
    
    UIButton* check_name_button_;
    UIButton* confirm_button_;
    UIButton* cancel_button_;

    float timer_;

    int32_t frame_index_;
};
