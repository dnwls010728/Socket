#include "pch.h"
#include "ScrollBox.h"

#include "imgui/imgui.h"
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

        float rect_height = child->GetRect().height;
        offset_y += rect_height;

        content_width_ = Math::Max(content_width_, child->GetRect().width);
        content_height_ += rect_height;
    }

    min_allowed_scroll_y_ = rect_.height - content_height_;
}

void ScrollBox::Tick(float delta_time)
{
    Widget::Tick(delta_time);

    if (ImGui::Begin("Property"))
    {
        ImGui::Text("Rect Height: %.2f", rect_.height);
        ImGui::Text("Scroll Offset Y: %.2f", scroll_offset_y_);
        ImGui::Text("Content Width: %.2f", content_width_);
        ImGui::Text("Content Height: %.2f", content_height_);
    }

    ImGui::End();
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
    if (kDelta.y != 0.f)
    {
        float scroll_speed = kDelta.y * 10.f;
        float previous_scroll_offset_y = scroll_offset_y_;

        scroll_offset_y_ = Math::Clamp(
            scroll_offset_y_ + scroll_speed,
            min_allowed_scroll_y_,
            0.f
        );

        float scroll_delta = scroll_offset_y_ - previous_scroll_offset_y;
        for (const auto& child : children_)
        {
            Math::Vector2 anchored_position = child->GetAnchoredPosition();
            anchored_position.y += scroll_delta;
            child->SetAnchoredPosition(anchored_position);
        }
    }
    
    return true;
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
