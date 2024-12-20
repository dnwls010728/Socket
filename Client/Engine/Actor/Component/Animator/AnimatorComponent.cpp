#include "pch.h"
#include "AnimatorComponent.h"

#include "Animation.h"
#include "AnimationPack.h"
#include "Actor/Actor.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

AnimatorComponent::AnimatorComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    renderer_(nullptr),
    animation_pack_(nullptr),
    current_animation_(nullptr),
    timer_(0.f),
    is_playing_(false),
    current_frame_(0)
{
}

void AnimatorComponent::PlayAnimation(const std::wstring& kName)
{
    if (!animation_pack_) return;

    const auto it = animation_pack_->animations_.find(kName);
    if (it == animation_pack_->animations_.end()) return;

    current_animation_ = it->second.get();
    current_frame_ = 0;
    timer_ = 0.f;
    is_playing_ = true;

    if (HasBegunPlay() && renderer_)
    {
        Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_pack_->target_);
        if (sprite) renderer_->SetSprite(sprite, current_animation_->frames_[0]);
    }
}

void AnimatorComponent::BeginPlay()
{
    ActorComponent::BeginPlay();

    ActorComponent* component = owner_->GetComponent(SpriteRendererComponent::StaticClass());
    if (component) renderer_ = static_cast<SpriteRendererComponent*>(component);

    if (renderer_ && animation_pack_ && current_animation_)
    {
        Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_pack_->target_);
        if (sprite) renderer_->SetSprite(sprite, current_animation_->frames_[0]);
    }
}

void AnimatorComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);
    if (!renderer_ || !is_playing_ || !current_animation_) return;

    const float frame_time = 1.f / current_animation_->frame_rate_;
    timer_ += delta_time;

    if (timer_ >= frame_time)
    {
        timer_ -= frame_time;
        if (current_frame_ >= current_animation_->frames_.size() - 1)
        {
            if (!current_animation_->is_loop_)
            {
                is_playing_ = false;
                return;
            }
        }
        
        current_frame_ = (current_frame_ + 1) % current_animation_->frames_.size();

        Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_pack_->target_);
        if (sprite) renderer_->SetSprite(sprite, current_animation_->frames_[current_frame_]);
    }
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
