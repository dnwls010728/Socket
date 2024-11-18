#include "pch.h"
#include "ScrollBox.h"

#include "Math/Color.h"
#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

ScrollBox::ScrollBox(const std::wstring& kName) :
    Widget(kName),
    content_width_(0.f),
    content_height_(0.f),
    scroll_offset_y_(0.f)
{
    is_ray_cast_target_ = true;
}

void ScrollBox::BeginPlay()
{
    Widget::BeginPlay();
    
    float offset_y = 0.f;
    for (const auto& child : children_)
    {
        child->SetAnchorPreset(AnchorPreset::kLeft | AnchorPreset::kTop, true);
        child->SetAnchoredPosition({0.f, offset_y});
        
        offset_y += child->GetRect().height;

        content_width_ = Math::Max(content_width_, child->GetRect().width);
        content_height_ += child->GetRect().height;
    }
}

void ScrollBox::Tick(float delta_time)
{
    Widget::Tick(delta_time);

    Mouse* mouse = Mouse::Get();

    if (is_hovered_)
    {
        int wheel_axis = mouse->GetWheelAxis();
        if (wheel_axis != 0)
        {
            float scroll_speed = wheel_axis * 30.f;
            float previous_scroll_offset_y = scroll_offset_y_;

            scroll_offset_y_ = Math::Clamp(
                scroll_offset_y_ + scroll_speed,
                rect_.height - content_height_,
                0.f
            );

            float scroll_delta = scroll_offset_y_ - previous_scroll_offset_y;
            if (scroll_delta != 0.f)
            {
                for (const auto& child : children_)
                {
                    Math::Vector2 anchored_position = child->GetAnchoredPosition();
                    anchored_position.y += scroll_delta;
                    child->SetAnchoredPosition(anchored_position);
                }
            }
        }
    }

}

void ScrollBox::Render()
{
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;
    
    Renderer* renderer = Renderer::Get();
    if (!renderer) return;

    renderer->BeginLayer(rect_);
    Widget::Render();
    renderer->EndLayer();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<ScrollBox>("ScrollBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
