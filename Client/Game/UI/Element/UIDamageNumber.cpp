#include "pch.h"
#include "UIDamageNumber.h"

#include "Asset/AssetManager.h"
#include "Math/Math.h"
#include "Windows/DX/UISprite.h"

UIDamageNumber::UIDamageNumber(const std::wstring& name) :
    UIElement(name),
    damage_(0),
    count_timer_(0.f),
    fade_timer_(0.f),
    alpha_(0.f)
{
    AssetManager* asset_manager = AssetManager::Get();
    
    number_sprite_ = asset_manager->Load<UISprite>(L"UI\\Number.png");
    miss_sprite_ = asset_manager->Load<UISprite>(L"UI\\Miss.png");
    
}

void UIDamageNumber::Tick(float delta_time)
{
    UIElement::Tick(delta_time);

    count_timer_ += delta_time;
    if (count_timer_ >= 1.f)
    {
        count_timer_ = 0.f;
        damage_++;
    }

    fade_timer_ += delta_time;
    if (fade_timer_ < 1.f)
    {
        float t = Math::Clamp(fade_timer_ / 1.f, 0.f, 1.f);
        alpha_ = 1.f - t;
    }
    else fade_timer_ = 0.f;
    
}

void UIDamageNumber::Render()
{
    UIElement::Render();

    Renderer* renderer = Renderer::Get();

    std::wstring damage = std::to_wstring(damage_);
    if (damage_ > 0)
    {
        for (size_t i = 0; i < damage.length(); ++i)
        {
            wchar_t digit = damage[i];
            std::wstring frame_name = L"Number_" + std::to_wstring(digit - L'0');
            renderer->DrawSimpleSprite(number_sprite_, frame_name, GetAbsolutePosition() + Math::Vector2(i * (20 * size_.x), 0), size_, alpha_);
        }
    }
    else
    {
        renderer->DrawSimpleSprite(miss_sprite_, L"Miss_0", GetAbsolutePosition(), size_, alpha_);
    }
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<UIDamageNumber>("UIDamageNumber")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_raw_ptr
        );
}
