#include "pch.h"
#include "ScrollBox.h"

#include "Math/Color.h"
#include "Math/Math.h"
#include "UI/Canvas.h"
#include "Windows/DX/Renderer.h"

ScrollBox::ScrollBox(const std::wstring& kName) :
    Widget(kName),
    content_width_(0.f),
    content_height_(0.f),
    scroll_offset_y_(0.f),
    min_allowed_scroll_y_(0.f)
{
}

void ScrollBox::BeginPlay()
{
    Widget::BeginPlay();

    float offset_y = 0.f;
    for (const auto& child : children_)
    {
        child->SetAnchorPreset(AnchorPreset::kLeft | AnchorPreset::kTop, true);
        child->SetAnchoredPosition({0.f, offset_y});
        
        offset_y += child->GetSize().y;

        content_width_ = Math::Max(content_width_, child->GetRect().width);
        content_height_ += child->GetRect().height;
    }

    min_allowed_scroll_y_ = size_.y - offset_y;
}

void ScrollBox::Tick(float delta_time)
{
    Widget::Tick(delta_time);

    float previous_scroll_offset_y = scroll_offset_y_;
    float target_scroll_y = scroll_offset_y_;

    if (scroll_offset_y_ > 0.f) target_scroll_y = 0.f;
    else if (scroll_offset_y_ < min_allowed_scroll_y_) target_scroll_y = min_allowed_scroll_y_;

    if (target_scroll_y != scroll_offset_y_)
    {
        scroll_offset_y_ = Math::Lerp(scroll_offset_y_, target_scroll_y, delta_time * 5.f);

        float scroll_delta = scroll_offset_y_ - previous_scroll_offset_y;
        UpdateChildrenPosition(scroll_delta);
    }
}

void ScrollBox::Render()
{
    Renderer* renderer = Renderer::Get();
    if (!renderer) return;
    
    WindowsWindow* window = World::Get()->GetWindow();
    if (!window) return;

    renderer->BeginLayer(rect_);
    Widget::Render();
    renderer->EndLayer();

    const float ratio = Canvas::Get()->GetScaleRatio();

    renderer->DrawBox(window, {rect_.x, rect_.y + scroll_offset_y_ * ratio, content_width_, content_height_}, {0, 0}, Math::Color::Red);
}

void ScrollBox::UpdateRect()
{
    Widget::UpdateRect();
    if (!has_begun_play_) return;

    content_width_ = 0.f;
    content_height_ = 0.f;

    for (const auto& child : children_)
    {
        content_width_ = Math::Max(content_width_, child->GetRect().width);
        content_height_ += child->GetRect().height;
    }
}

bool ScrollBox::OnScroll(const Math::Vector2& kPosition, const Math::Vector2& kDelta)
{
    if (rect_.y >= content_height_) return Widget::OnScroll(kPosition, kDelta);
    
    if (kDelta.y != 0.f)
    {
        float scroll_speed = kDelta.y * 10.f;
        float previous_scroll_offset_y = scroll_offset_y_;

        scroll_offset_y_ += scroll_speed;

        float scroll_delta = scroll_offset_y_ - previous_scroll_offset_y;
        UpdateChildrenPosition(scroll_delta);
    }
    
    return true;
}

void ScrollBox::UpdateChildrenPosition(float scroll_delta)
{
    if (scroll_offset_y_ != 0.f)
    {
        for (const auto& child : children_)
        {
            Math::Vector2 anchored_position = child->GetAnchoredPosition();
            anchored_position.y += scroll_delta;
            child->SetAnchoredPosition(anchored_position);
        }
    }
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
