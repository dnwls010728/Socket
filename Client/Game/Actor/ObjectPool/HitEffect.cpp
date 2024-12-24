#include "pch.h"
#include "HitEffect.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimationPack.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

HitEffect::HitEffect(const std::wstring& kName) :
    PooledObject(kName)
{
    sprite_ = AssetManager::Get()->Load<Sprite>(L"Sprites\\Effect\\Hit\\HitEffect.png");
    animation_pack_ = AssetManager::Get()->Load<AnimationPack>(L"Sprites\\Effect\\Hit\\HitEffect.png.animpack");
    
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetSprite(sprite_, L"HitEffect_0");

    animator_ = AddComponent<AnimatorComponent>(L"Animator");
    animator_->SetAnimationPack(animation_pack_);
    animator_->OnEndHandler.Add(this, &HitEffect::OnPlayEnd);
}

void HitEffect::OnEnable()
{
    PooledObject::OnEnable();
    
    animator_->PlayAnimation(L"Effect");
}

void HitEffect::OnPlayEnd()
{
    Deactivate();
}

RTTR_REGISTRATION
{
    using namespace rttr;
    
    registration::class_<HitEffect>("HitEffect")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
