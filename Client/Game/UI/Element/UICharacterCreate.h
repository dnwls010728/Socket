#pragma once
#include "Math/Color.h"
#include "UI/UIContainer.h"

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

private:
    void OnColorChanged(Math::Color color);
    
    UIImage* background_;

    UIColorPicker* color_picker_;
};
