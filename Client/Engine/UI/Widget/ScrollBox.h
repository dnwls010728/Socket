#pragma once
#include "UI/Widget.h"

class ScrollBox : public Widget
{
    SHADER_CLASS_HELPER(ScrollBox)
    GENERATED_BODY(ScrollBox, Widget)
    
public:
    ScrollBox(const std::wstring& kName);
    virtual ~ScrollBox() override = default;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float delta_time) override;
    virtual void Render() override;
    virtual void UpdateRect() override;

private:
    float content_width_;
    float content_height_;
    float scroll_offset_x_;
    float scroll_offset_y_;

    Math::Rect thumb_rect_;
    
};
