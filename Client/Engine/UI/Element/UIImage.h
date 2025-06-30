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

    FORCEINLINE void SetSprite(UISprite* ui_sprite) { ui_sprite_ = ui_sprite; }

protected:
    virtual void Tick(float delta_time) override;
    virtual void Render() override;

    UISprite* ui_sprite_;

    // 테스트
    float timer_;
    int32_t frame_index_;
    
};
