#include "pch.h"
#include "Effect.h"

#include "Actor/Component/SpriteRendererComponent.h"
#include "Actor/Component/Animator/AnimatorComponent.h"
#include "Asset/AssetManager.h"

Effect::Effect(const std::wstring& name) :
    NetworkActor(name)
{
    renderer_ = AddComponent<SpriteRendererComponent>(L"SpriteRenderer");
    renderer_->SetFlipX(true);
    renderer_->SetZOrder(std::numeric_limits<int16_t>::max());

    animator_ = AddComponent<AnimatorComponent>(L"Animator");
}

void Effect::SetFlipX(bool flipX)
{
    renderer_->SetFlipX(!flipX);
}

void Effect::BeginPlay()
{
    NetworkActor::BeginPlay();
    
    AnimationPack* animation_pack = AssetManager::Get()->Load<AnimationPack>(animation_pack_);
    if (animation_pack)
    {
        animator_->SetAnimationPack(animation_pack);
        animator_->PlayAnimation(animation_);
    }
}

void Effect::Tick(float DeltaTime)
{
    NetworkActor::Tick(DeltaTime);

    if (!animator_->IsPlaying()) Destroy();
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Effect>("Effect")
        .constructor<const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
