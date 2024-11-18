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

    scroll_offset_y_ -= 100.f * delta_time;

    for (const auto& child : children_)
    {
        Math::Vector2 position = child->GetAnchoredPosition();
        position.y -= 100.f * delta_time;

        child->SetAnchoredPosition(position);
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

    Math::Rect content_rect = {rect_.x, rect_.y + scroll_offset_y_, content_width_, content_height_};
    renderer->DrawBox(window, content_rect, Math::Vector2::Zero(), Math::Color::Red, angle_, 1.f);
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
