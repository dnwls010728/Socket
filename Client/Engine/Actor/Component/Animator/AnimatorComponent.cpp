#include "pch.h"
#include "AnimatorComponent.h"

#include "Actor/Actor.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

AnimatorComponent::AnimatorComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    renderer_(nullptr),
    timer_(0.f),
    current_index_(0),
    is_playing_(false),
    animation_set_(nullptr),
    current_sequence_()
{
}

void AnimatorComponent::SetAnimationSet(AnimationSet* animation_set)
{
    animation_set_ = animation_set;
}

void AnimatorComponent::PlayAnimation(const std::wstring& kSequence)
{
    if (animation_set_)
    {
        if (current_sequence_.name == kSequence) return;
        
        const auto& sequences = animation_set_->GetSequences();
        auto it = sequences.find(kSequence);
        if (it != sequences.end())
        {
            timer_ = 0.f;
            current_index_ = 0;
            current_sequence_ = it->second;
            is_playing_ = true;
        }
    }
}

void AnimatorComponent::BeginPlay()
{
    ActorComponent::BeginPlay();

    ActorComponent* component = GetOwner()->GetComponent(SpriteRendererComponent::StaticClass());
    if (component)
    {
        renderer_ = static_cast<SpriteRendererComponent*>(component);
        if (renderer_)
        {
            Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_set_->GetTarget());
            if (sprite) renderer_->SetSprite(sprite, current_sequence_.frames[current_index_]);
        }
    }
}

void AnimatorComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    if (is_playing_)
    {
        timer_ += delta_time;
        if (timer_ >= 1.f / current_sequence_.sample_frame_rate)
        {
            if (current_index_ < current_sequence_.frames.size())
            {
                current_index_ = (current_index_ + 1) % current_sequence_.frames.size();
                
                Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_set_->GetTarget());
                if (sprite) renderer_->SetSprite(sprite, current_sequence_.frames[current_index_]);
            }
            else
            {
                if (current_sequence_.is_loop) current_index_ = 0;
                else is_playing_ = false;
            }

            timer_ -= 1.f / current_sequence_.sample_frame_rate;
        }
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
