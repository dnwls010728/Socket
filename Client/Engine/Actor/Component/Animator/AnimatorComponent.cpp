#include "pch.h"
#include "AnimatorComponent.h"

#include "Actor/Actor.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Windows/DX/Sprite.h"

AnimatorComponent::AnimatorComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    renderer_(nullptr),
    sprite_(nullptr),
    timer_(0.f),
    current_index_(0)
{
}

void AnimatorComponent::BeginPlay()
{
    ActorComponent::BeginPlay();

    ActorComponent* component = GetOwner()->GetComponent(SpriteRendererComponent::StaticClass());
    if (component)
    {
        renderer_ = static_cast<SpriteRendererComponent*>(component);
        if (renderer_) sprite_ = renderer_->GetSprite();
    }
}

void AnimatorComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);
    if (!renderer_) return;

    const SpriteAnimation& animation = sprite_->GetAnimations().at(L"Walk");
    if (timer_ >= 1.f / animation.sample_frame_rate)
    {
        current_index_ = (current_index_ + 1) % animation.frame_indexes.size();
        renderer_->SetFrame(animation.frame_indexes[current_index_]);
        timer_ -= 1.f / animation.sample_frame_rate;
    }

    timer_ += delta_time;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<AnimatorComponent>("AnimatorComponent")
        .constructor<Actor*, const std::wstring&>()
        (
            policy::ctor::as_std_shared_ptr
        );
}
