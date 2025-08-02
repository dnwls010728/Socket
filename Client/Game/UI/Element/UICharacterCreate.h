#pragma once
#include "Math/Color.h"
#include "UI/UIContainer.h"
#include "UI/Element/UIText.h"

class UIColorPicker;
class UIEditableText;
class UIImage;

class UICharacterCreate : public UIContainer
{
    GENERATED_BODY(UICharacterCreate, UIContainer)
    
public:
    UICharacterCreate(const std::wstring& name);
    virtual ~UICharacterCreate() override = default;

protected:
    virtual void Init() override;
    virtual void Render() override;

private:
    void OnColorChanged(Math::Color color);
    
    UIImage* background_;
    UIImage* character_;

    UIText* title_text_;

    UIColorPicker* color_picker_;
};
