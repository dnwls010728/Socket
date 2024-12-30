#pragma once
#include "UI_OLD/Widget.h"

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

    virtual bool OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta) override;

private:
    void UpdateChildrenPosition(float scroll_delta);
    
    float content_width_;
    float content_height_;

    float scroll_offset_y_;
    float min_allowed_scroll_y_;
    
};
