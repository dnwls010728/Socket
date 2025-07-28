#include "pch.h"
#include "UIScrollBox.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIScrollBox::UIScrollBox(const std::wstring& name) :
    UIMask(name),
    vertical_alignment_(VerticalAlignment::kBottom),
    spacing_(0.f),
    dirty_(false)
{
    content_ = AddChild<UIContainer>(UIContainer::StaticClass(), L"Content");
}

void UIScrollBox::Init()
{
    UIMask::Init();
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
    renderer->DrawBox(GetAbsolutePosition(), size_, Math::Color::Red);
    renderer->DrawBox(content_->GetAbsolutePosition(), content_->GetSize(), Math::Color::Green);
    
    UIMask::Render();
}

void UIScrollBox::UpdateLayout() const
{
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
    
    if (vertical_alignment_ == VerticalAlignment::kBottom)
    {
        content_->SetRelativePosition({0.f, size_.y - content_->GetSize().y});
    }
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
