#include "pch.h"
#include "ScrollBox.h"

#include "Logger.h"
#include "Math/Color.h"
#include "Math/Math.h"
#include "UI/Canvas.h"
#include "Windows/DX/Renderer.h"

ScrollBox::ScrollBox(const std::wstring& kName) :
    Widget(kName),
    content_width_(0.f),
    content_height_(0.f),
    scroll_offset_x_(0.f),
    scroll_offset_y_(0.f),
    thumb_rect_(Math::Rect::Zero())
{
    is_ray_cast_target_ = true;
}

void ScrollBox::SetScrollOffsetX(float offset_x)
{
    offset_x = Math::Clamp(offset_x, 0.f, 1.f);
    
    float previous_scroll_offset_x = scroll_offset_x_;
    scroll_offset_x_ = Math::Lerp(0.f, content_width_ - rect_.width, -offset_x);

    float scroll_delta = scroll_offset_x_ - previous_scroll_offset_x;
    if (scroll_delta != 0.f)
    {
        for (const auto& child : children_)
        {
            Math::Vector2 anchored_position = child->GetAnchoredPosition();
            anchored_position.x += scroll_delta / Canvas::Get()->GetScaleRatio();
            child->SetAnchoredPosition(anchored_position);
        }
    }
}

void ScrollBox::SetScrollOffsetY(float offset_y)
{
    offset_y = Math::Clamp(offset_y, 0.f, 1.f);
    
    float previous_scroll_offset_y = scroll_offset_y_;
    scroll_offset_y_ = Math::Lerp(0.f, content_height_ - rect_.height, -offset_y);

    float scroll_delta = scroll_offset_y_ - previous_scroll_offset_y;
    if (scroll_delta != 0.f)
    {
        for (const auto& child : children_)
        {
            Math::Vector2 anchored_position = child->GetAnchoredPosition();
            anchored_position.y += scroll_delta / Canvas::Get()->GetScaleRatio();
            child->SetAnchoredPosition(anchored_position);
        }
    }
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

    SetScrollOffsetX(0.f);
    SetScrollOffsetY(1.f);
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

    Canvas* canvas = Canvas::Get();
    const float scale_ratio = canvas->GetScaleRatio();

    if (content_width_ > rect_.width)
    {
        float thumb_ratio = rect_.width / content_width_;
        float thumb_width = Math::Max(rect_.width * thumb_ratio, 20.f);
        float thumb_x = Math::Lerp(0.f, rect_.width - thumb_width, -scroll_offset_x_ / (content_width_ - rect_.width));
        renderer->DrawSolidRoundBox(window, {rect_.x + thumb_x, rect_.y + rect_.height - 10.f * scale_ratio, thumb_width, 10.f * scale_ratio}, {0.f, 0.f}, Math::Color::Gray, 5.f);
    }

    if (content_height_ > rect_.height)
    {
        float thumb_ratio = rect_.height / content_height_;
        float thumb_height = Math::Max(rect_.height * thumb_ratio, 20.f);
        float thumb_y = Math::Lerp(0.f, rect_.height - thumb_height, -scroll_offset_y_ / (content_height_ - rect_.height));
        renderer->DrawSolidRoundBox(window, {rect_.x + rect_.width - 10.f * scale_ratio, rect_.y + thumb_y, 10.f * scale_ratio, thumb_height}, {0.f, 0.f}, Math::Color::Gray, 5.f);
    }
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

    if (content_width_ > rect_.width)
    {
        float previous_scroll_offset_x = scroll_offset_x_;
        scroll_offset_x_ = Math::Clamp(scroll_offset_x_, rect_.width - content_width_, 0.f);
        if (scroll_offset_x_ != previous_scroll_offset_x)
        {
            for (const auto& child : children_)
            {
                Math::Vector2 anchored_position = child->GetAnchoredPosition();
                anchored_position.x += (scroll_offset_x_ - previous_scroll_offset_x) / Canvas::Get()->GetScaleRatio();
                child->SetAnchoredPosition(anchored_position);
            }
        }
    }

    if (content_height_ > rect_.height)
    {
        float previous_scroll_offset_y = scroll_offset_y_;
        scroll_offset_y_ = Math::Clamp(scroll_offset_y_, rect_.height - content_height_, 0.f);
        if (scroll_offset_y_ != previous_scroll_offset_y)
        {
            for (const auto& child : children_)
            {
                Math::Vector2 anchored_position = child->GetAnchoredPosition();
                anchored_position.y += (scroll_offset_y_ - previous_scroll_offset_y) / Canvas::Get()->GetScaleRatio();
                child->SetAnchoredPosition(anchored_position);
            }
        }
    }
}

void ScrollBox::OnScroll(float x, float y)
{
    Canvas* canvas = Canvas::Get();
    const float scale_ratio = canvas->GetScaleRatio();
    
    if (content_width_ > rect_.width)
    {
        if (x != 0.f)
        {
            float scroll_speed = -x * 30.f; 
            float previous_scroll_offset_x = scroll_offset_x_;

            scroll_offset_x_ = Math::Clamp(
                scroll_offset_x_ + scroll_speed,
                rect_.width - content_width_,
                0.f
            );

            float scroll_delta = scroll_offset_x_ - previous_scroll_offset_x;
            if (scroll_delta != 0.f)
            {
                for (const auto& child : children_)
                {
                    Math::Vector2 anchored_position = child->GetAnchoredPosition();
                    anchored_position.x += scroll_delta / scale_ratio;
                    child->SetAnchoredPosition(anchored_position);
                }
            }
        }
    }
    
    if (content_height_ > rect_.height)
    {
        if (y != 0.f)
        {
            float scroll_speed = y * 30.f; 
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
                    anchored_position.y += scroll_delta / scale_ratio;
                    child->SetAnchoredPosition(anchored_position);
                }
            }
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
