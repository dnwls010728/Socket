#include "pch.h"
#include "UIScrollBox.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIScrollBox::UIScrollBox(const std::wstring& name) :
    UIMask(name),
    vertical_alignment_(VerticalAlignment::kTop),
    min_offset_(0.f),
    max_offset_(0.f),
    scroll_offset_(0.f),
    scroll_step_(0.f),
    spacing_(0.f),
    dirty_(false)
{
    content_ = AddChild<UIContainer>(UIContainer::StaticClass(), L"Content");
}

void UIScrollBox::RemoveItem(UIElement* item)
{
    content_->RemoveChild(item);
    dirty_ = true;
}

void UIScrollBox::SetVerticalAlignment(VerticalAlignment alignment)
{
    vertical_alignment_ = alignment;
    dirty_ = true;
}

void UIScrollBox::SetSpacing(float spacing)
{
    spacing_ = spacing;
    dirty_ = true;
}

void UIScrollBox::Tick(float delta_time)
{
    UIMask::Tick(delta_time);

    if (dirty_)
    {
        UpdateLayout();
        dirty_ = false;
    }
}

void UIScrollBox::Render()
{
    Renderer* renderer = Renderer::Get();
    // renderer->DrawBox(GetAbsolutePosition(), size_, Math::Color::Red);
    // renderer->DrawBox(content_->GetAbsolutePosition(), content_->GetSize(), Math::Color::Green);
    
    UIMask::Render();

    // 스크롤 가능하지 않으면 바 표시 안 함
    const float view_h    = GetSize().y;
    const float content_h = content_->GetSize().y;
    if (content_h <= view_h) return;

    const float denom = (max_offset_ - min_offset_);
    if (Math::Abs(denom) < 1e-6f) return;

    // 트랙/엄지 기하 정의
    const float thickness   = 8.f;   // 바 두께
    const float margin      = 2.f;   // 위/아래 여백
    const float track_h     = Math::Max(view_h - margin * 2.f, 0.f);
    const float min_thumb_h = 16.f;  // 최소 엄지 높이

    // 엄지 높이: viewport/content 비율
    float thumb_h = track_h * (view_h / content_h);
    thumb_h = Math::Clamp(thumb_h, min_thumb_h, track_h);

    // 정규화된 스크롤 (0..1)
    float t = (scroll_offset_ - min_offset_) / denom;
    t = Math::Clamp(t, 0.f, 1.f);

    // 위치 계산 (컨트롤 오른쪽에 표시)
    const Math::Vector2 abs_pos = GetAbsolutePosition();
    const float track_x = abs_pos.x + GetSize().x - thickness - 1.f; // 오른쪽에서 약간 띄우기
    const float track_y = abs_pos.y + margin;

    const float thumb_travel = track_h - thumb_h;
    const float thumb_y = track_y + thumb_travel * t;

    // 그리기
    // 트랙
    renderer->DrawBox({track_x, track_y}, {thickness, track_h}, Math::Color::Red);
    // 엄지
    renderer->DrawSolidBox({track_x, thumb_y}, {thickness, thumb_h}, Math::Color::Blue);

    // 선택: 위/아래 그라데이션 페이드(컨텐츠가 마스크 속에 있다는 느낌)
    // 필요하면 여기에 반투명 그라데이션 쿼드 추가
}

bool UIScrollBox::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    float amount = -delta.y * scroll_step_;
    scroll_offset_ = Math::Clamp(scroll_offset_ + amount, min_offset_, max_offset_);
    
    float base_y = 0.f;
    if (vertical_alignment_ == VerticalAlignment::kBottom)
        base_y = GetSize().y - content_->GetSize().y;

    content_->SetRelativePosition({0.f, base_y - scroll_offset_});
    return true;
}

void UIScrollBox::UpdateLayout()
{
    const float previous_height = content_->GetSize().y;
    const float previous_max = max_offset_;
    const bool was_at_bottom = Math::Abs(scroll_offset_ - previous_max) <= 1.f;

    Math::Vector2 content_size = Math::Vector2::Zero();
    float layout_y = 0.f;
    const auto& children = content_->GetChildren();
    for (int32_t i = 0; i < children.size(); ++i)
    {
        UIElement* child = children[i].get();
        if (!child || !child->IsActive()) continue;

        child->SetRelativePosition({0.f, layout_y});

        content_size.x = Math::Max(content_size.x, child->GetSize().x);
        content_size.y += child->GetSize().y + spacing_;

        layout_y += child->GetSize().y + spacing_;
    }

    content_size.y = Math::Max(content_size.y - spacing_, 0.f);
    content_->SetSize(content_size);

    if (content_size.y <= GetSize().y)
    {
        min_offset_ = 0.f;
        max_offset_ = 0.f;
        scroll_offset_ = 0.f;
    }
    else if (vertical_alignment_ == VerticalAlignment::kBottom)
    {
        min_offset_ = GetSize().y - content_size.y;
        max_offset_ = 0.f;
    }
    else
    {
        min_offset_ = 0.f;
        max_offset_ = content_size.y - GetSize().y;
    }

    const float dh = content_size.y - previous_height;
    if (vertical_alignment_ == VerticalAlignment::kBottom)
    {
        if (was_at_bottom) scroll_offset_ = max_offset_;
        else scroll_offset_ -= dh;
    }

    scroll_offset_ = Math::Clamp(scroll_offset_, min_offset_, max_offset_);
    
    float base_y = 0.f;
    if (vertical_alignment_ == VerticalAlignment::kBottom)
        base_y = GetSize().y - content_->GetSize().y;
    
    content_->SetRelativePosition({0.f, base_y - scroll_offset_});
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIScrollBox>("UIScrollBox")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
