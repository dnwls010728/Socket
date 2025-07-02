#include "pch.h"
#include "Damage.h"

#include "Actor/Component/TransformComponent.h"
#include "Asset/AssetManager.h"
#include "Components/DamageRendererComponent.h"
#include "Math/Math.h"
#include "Windows/DX/Sprite.h"

Damage::Damage(const std::wstring& name) :
    Actor(name),
    fade_timer_(0.f)
{
    damage_renderer_ = AddComponent<DamageRendererComponent>(L"DamageRenderer");

    Sprite* damage_sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Damage.png");
    if (damage_sprite) damage_renderer_->SetDamageSprite(damage_sprite);

    Sprite* miss_sprite = AssetManager::Get()->Load<Sprite>(L"Sprites\\Miss.png");
    if (miss_sprite) damage_renderer_->SetMissSprite(miss_sprite);

    damage_renderer_->SetDamage(1000);
    
}

void Damage::Tick(float delta_time)
{
    Actor::Tick(delta_time);

    GetTransform()->Translate(Math::Vector2::Up() * delta_time);

    fade_timer_ += delta_time;
    if (fade_timer_ < 1.f)
    {
        float t = 1.f - Math::Clamp(fade_timer_ / 1.f, 0.f, 1.f);

        Math::Color color = damage_renderer_->GetColor();
        color.a = static_cast<uint8_t>(t * 255.f);
        damage_renderer_->SetColor(color);
    }
    else Destroy();
    
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Damage>("Damage")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
