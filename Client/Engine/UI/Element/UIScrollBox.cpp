#include "pch.h"
#include "UIScrollBox.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIScrollBox::UIScrollBox(const std::wstring& name) :
    UIMask(name),
    vertical_alignment_(VerticalAlignment::kBottom),
    min_offset_(0.f),
    max_offset_(0.f),
    scroll_offset_(0.f),
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
}

bool UIScrollBox::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    float amount = -delta.y * 36.f;
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
