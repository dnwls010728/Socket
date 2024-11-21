#pragma once
#include "UI/Widget.h"

class ScrollBox : public Widget
{
    SHADER_CLASS_HELPER(ScrollBox)
    GENERATED_BODY(ScrollBox, Widget)
    
public:
    ScrollBox(const std::wstring& kName);
    virtual ~ScrollBox() override = default;

    void SetScrollOffsetX(float offset_x);
    void SetScrollOffsetY(float offset_y);

protected:
    virtual void BeginPlay() override;
    virtual void Render() override;
    virtual void UpdateRect() override;

private:
    friend class Canvas;
    
    void OnScroll(float x, float y);
    
    float content_width_;
    float content_height_;
    float scroll_offset_x_;
    float scroll_offset_y_;

    Math::Rect thumb_rect_;
    
};
