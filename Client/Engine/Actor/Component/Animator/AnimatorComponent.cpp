#include "pch.h"
#include "AnimatorComponent.h"

#include "AnimationPack.h"
#include "AnimationTransition.h"
#include "Actor/Actor.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

AnimatorComponent::StateNode::StateNode(const std::wstring& name) :
    name_(name),
    animation_(),
    transitions_()
{
}

void AnimatorComponent::StateNode::AddTransition(const std::wstring& kTo, const std::shared_ptr<Condition>& kCondition)
{
    transitions_.emplace(std::make_shared<Transition>(kTo, kCondition));
}

AnimatorComponent::AnimatorComponent(Actor* owner, const std::wstring& kName) :
    ActorComponent(owner, kName),
    renderer_weak_ptr_(),
    animation_pack_(nullptr),
    timer_(0.f),
    is_playing_(false),
    current_frame_(0),
    current_state_(nullptr),
    nodes_(),
    any_transitions_(),
    parameters_()
{
}

void AnimatorComponent::AddTransition(const std::wstring& kFrom, const std::wstring& kTo, const std::shared_ptr<Condition>& kCondition)
{
    GetOrAddNode(kFrom)->AddTransition(kTo, kCondition);
}

void AnimatorComponent::AddAnyTransition(const std::wstring& kTo, const std::shared_ptr<Condition>& kCondition)
{
    any_transitions_.emplace(std::make_shared<Transition>(kTo, kCondition));
}

void AnimatorComponent::PlayAnimation(const std::wstring& kName)
{
    std::shared_ptr<StateNode> state = GetOrAddNode(kName);

    // if (!state || !HasBegunPlay()) return;
    if (!state) return;
    if (current_state_ == state && is_playing_) return;
    
    is_playing_ = false;

    current_state_ = state;
    
    current_frame_ = 0;
    timer_ = 0.f;

    const Animation& animation = current_state_->GetAnimation();
    if (animation.frame.empty()) return;

    std::shared_ptr<SpriteRendererComponent> renderer = renderer_weak_ptr_.lock();
    if (renderer)
    {
        Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_pack_->target_);
        if (sprite) renderer->SetSprite(sprite, animation.frame.front());
    }
    
    is_playing_ = true;
}

void AnimatorComponent::StopAnimation()
{
    is_playing_ = false;
    current_state_ = nullptr;
}

void AnimatorComponent::SetBool(const std::wstring& kName, bool value)
{
    parameters_[kName] = value;
}

void AnimatorComponent::SetFloat(const std::wstring& kName, float value)
{
    parameters_[kName] = value;
}

void AnimatorComponent::SetInt(const std::wstring& kName, int32_t value)
{
    parameters_[kName] = value;
}

void AnimatorComponent::SetTrigger(const std::wstring& kName)
{
    parameters_[kName] = true;
}

bool AnimatorComponent::GetBool(const std::wstring& kName)
{
    if (std::get_if<bool>(&parameters_[kName]))
        return std::get<bool>(parameters_[kName]);

    return false;
}

bool AnimatorComponent::GetTrigger(const std::wstring& kName)
{
    if (std::get_if<bool>(&parameters_[kName]))
    {
        if (std::get<bool>(parameters_[kName]))
        {
            parameters_[kName] = false;
            return true;
        }
    }
    
    return false;
}

float AnimatorComponent::GetFloat(const std::wstring& kName)
{
    if (std::get_if<float>(&parameters_[kName]))
        return std::get<float>(parameters_[kName]);

    return 0.f;
}

int32_t AnimatorComponent::GetInt(const std::wstring& kName)
{
    if (std::get_if<int32_t>(&parameters_[kName]))
        return std::get<int32_t>(parameters_[kName]);

    return 0;
}

std::shared_ptr<AnimatorComponent::StateNode> AnimatorComponent::GetOrAddNode(const std::wstring& name)
{
    std::shared_ptr<StateNode> node = nullptr;

    const auto it = nodes_.find(name);
    if (it != nodes_.end()) node = it->second;

    if (!node)
    {
        node = std::make_shared<StateNode>(name);
        nodes_[name] = node;
    }

    return node;
}

void AnimatorComponent::SetAnimationPack(AnimationPack* animation_pack)
{
    animation_pack_ = animation_pack;
    if (!animation_pack_) return;

    is_playing_ = false;
    current_state_ = nullptr;

    nodes_.clear();
    any_transitions_.clear();

    const auto& animations = animation_pack_->animations_;
    for (const auto& animation : animations)
    {
        GetOrAddNode(animation.first)->SetAnimation(animation.second);
    }
}

void AnimatorComponent::BeginPlay()
{
    ActorComponent::BeginPlay();

    renderer_weak_ptr_ = owner_->GetComponent<SpriteRendererComponent>(SpriteRendererComponent::StaticClass());
}

void AnimatorComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    std::shared_ptr<Transition> transition = GetTransition();
    if (transition) PlayAnimation(transition->GetTo());
    
    if (!current_state_ || !is_playing_) return;
    
    const Animation& animation = current_state_->GetAnimation();
    if (animation.frame.empty()) return;

    std::shared_ptr<SpriteRendererComponent> renderer = renderer_weak_ptr_.lock();
    if (!renderer) return;

    const float frame_time = 1.f / animation.frame_rate;
    timer_ += delta_time;

    if (timer_ >= frame_time)
    {
        timer_ -= frame_time;
        if (current_frame_ >= animation.frame.size() - 1)
        {
            if (!animation.is_loop)
            {
                if (OnEndHandler.IsBound()) OnEndHandler.Execute();
                is_playing_ = false;
                return;
            }
        }

        current_frame_ = (current_frame_ + 1) % animation.frame.size();

        Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_pack_->target_);
        if (sprite) renderer->SetSprite(sprite, animation.frame[current_frame_]);
    }
}

std::shared_ptr<Transition> AnimatorComponent::GetTransition()
{
    for (const auto& kTransition : any_transitions_)
    {
        if (kTransition->CheckCondition(this)) return kTransition;
    }

    if (current_state_)
    {
        for (const auto& kTransition : current_state_->GetTransitions())
        {
            if (kTransition->CheckCondition(this)) return kTransition;
        }
    }
    
    return nullptr;
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
