#include "pch.h"
#include "UIPalette.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIPalette::UIPalette(const std::wstring& name) :
    UIContainer(name),
    h_(0.f),
    s_(0.f),
    v_(0.f),
    color_(Math::Color::White),
    value_changed_event_([](Math::Color color) {})
{
    SetSize({200.f, 200.f});
}

void UIPalette::OnValueChanged(void(* func)(const Math::Color& color))
{
    value_changed_event_ = func;
}

void UIPalette::SetHue(float hue)
{
    h_ = Math::Clamp(hue, 0.f, 360.f);
    UpdateColor();
}

void UIPalette::Init()
{
    UIContainer::Init();
    UpdateColor(false);
}

void UIPalette::Render()
{
    Renderer* renderer = Renderer::Get();
    UIContainer::Render();
    
    renderer->DrawSolidBox(GetAbsolutePosition(), {200.f, 200.f}, Math::Color::White);
    renderer->DrawGradientSolidBox(GetAbsolutePosition(), {200.f, 200.f}, {255, 255, 255, 0}, Math::Color::HSVToColor(360.f - h_, 1.f, 1.f), GradientDirection::kHorizontal);
    renderer->DrawGradientSolidBox(GetAbsolutePosition(), {200.f, 200.f}, {255, 255, 255, 0}, {0, 0, 0, 255}, GradientDirection::kVertical);

    Math::Vector2 position = GetAbsolutePosition();
    position.x += s_ * GetSize().x - 5.f;
    position.y += v_ * GetSize().y - 5.f;
    renderer->DrawBox(position, {10.f, 10.f}, Math::Color::White, 2.f);
}

bool UIPalette::OnDragBegin(const Math::Vector2& position)
{
    return true;
}

bool UIPalette::OnDrag(const Math::Vector2& position, const Math::Vector2& delta)
{
    s_ = Math::Clamp01((position.x - GetAbsolutePosition().x) / GetSize().x);
    v_ = Math::Clamp01((position.y - GetAbsolutePosition().y) / GetSize().y);
    UpdateColor();
    return true;
}

bool UIPalette::OnDragEnd(const Math::Vector2& position)
{
    return true;
}

void UIPalette::UpdateColor(bool is_notify)
{
    color_ = Math::Color::HSVToColor(360.f - h_, s_, 1.f - v_);
    if (is_notify) value_changed_event_(color_);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIPalette>("UIPalette")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
