#include "pch.h"
#include "UIDeathFadeIn.h"

#include "Math/Math.h"
#include "Windows/DX/Renderer.h"

UIDeathFadeIn::UIDeathFadeIn(const std::wstring& name) :
    UIElement(name),
    fade_timer_(0.f),
    color_(Math::Color::Black)
{
    EngineSettings* settings = EngineSettings::Get();
    
    float screen_height = settings->GetScreenHeight();
    float screen_width = settings->GetScreenWidth();
    
    SetSize({ screen_width, screen_height });
}

void UIDeathFadeIn::Reset()
{
    color_.a = 0;
    fade_timer_ = 0.f;
}

void UIDeathFadeIn::Tick(float delta_time)
{
    UIElement::Tick(delta_time);

    if (fade_timer_ < 1.f)
    {
        fade_timer_ += delta_time;

        float t = Math::Clamp(fade_timer_ / 1.f, 0.f, .8f);
        color_.a = static_cast<uint8_t>(t * 255.f);
    }
}

void UIDeathFadeIn::Render()
{
    UIElement::Render();

    Renderer::Get()->DrawSolidBox(GetAbsolutePosition(), GetSize(), color_);
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIDeathFadeIn>("UIDeathFadeIn")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
