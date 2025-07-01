#pragma once
#include <d2d1.h>

#include "UI/UIElement.h"

class UISprite;

class UIImage : public UIElement
{
    GENERATED_BODY(UIImage, UIElement)
    
public:
    UIImage(const std::wstring& name);
    virtual ~UIImage() override = default;

    void SetSprite(UISprite* ui_sprite, const std::wstring& frame_name);

protected:
    virtual void Render() override;

    UISprite* ui_sprite_;

    std::wstring current_frame_;
    
};
