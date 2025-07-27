#include "pch.h"
#include "UIScrollBox.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIScrollBox::UIScrollBox(const std::wstring& name) :
    UIMask(name),
    target_offset_(0.f),
    offset_(0.f),
    min_offset_(0.f),
    max_offset_(0.f)
{
    container_ = AddChild<UIContainer>(UIContainer::StaticClass(), L"ScrollContainer");
    container_->SetSize({ 292.f, 25.f });
}

void UIScrollBox::Init()
{
    UIMask::Init();
    
    float dy = container_->GetSize().y - size_.y;
    dy = Math::Max(dy, 0.f);

    min_offset_ = 0.f;
    max_offset_ = dy;
}

void UIScrollBox::Tick(float delta_time)
{
    UIMask::Tick(delta_time);
    
    float alpha = 1.f - std::exp(-12.5f * delta_time);
    offset_ += (target_offset_ - offset_) * alpha;
    container_->SetRelativePosition({0.f, -offset_});
}

void UIScrollBox::Render()
{
    Renderer* renderer = Renderer::Get();
    renderer->DrawBox(GetAbsolutePosition(), size_, Math::Color::Red);
    renderer->DrawBox(container_->GetAbsolutePosition(), container_->GetSize(), Math::Color::Green);
    
    UIMask::Render();
}

bool UIScrollBox::OnScroll(const Math::Vector2& position, const Math::Vector2& delta)
{
    target_offset_ = Math::Clamp(target_offset_ - delta.y * 25.f, min_offset_, max_offset_);
    return true;
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
