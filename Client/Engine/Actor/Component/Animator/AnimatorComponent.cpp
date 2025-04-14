#include "pch.h"
#include "AnimatorComponent.h"

#include "Animation.h"
#include "AnimationPack.h"
#include "AnimationTransition.h"
#include "Actor/Actor.h"
#include "Actor/Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Windows/DX/Sprite.h"

AnimatorComponent::StateNode::StateNode(const std::wstring& kState) :
    state_(kState),
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
    current_animation_(nullptr),
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
    if (!animation_pack_) return;

    const auto it = animation_pack_->animations_.find(kName);
    if (it == animation_pack_->animations_.end()) return;

    current_animation_ = it->second.get();
    current_frame_ = 0;
    timer_ = 0.f;
    is_playing_ = true;

    std::shared_ptr<SpriteRendererComponent> renderer_ptr = renderer_weak_ptr_.lock();
    if (HasBegunPlay() && renderer_ptr)
    {
        Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_pack_->target_);
        if (sprite) renderer_ptr->SetSprite(sprite, current_animation_->frames_[0]);
    }
}

void AnimatorComponent::SetBool(const std::wstring& kName, bool value)
{
    parameters_[kName] = value;
}

void AnimatorComponent::SetFloat(const std::wstring& kName, float value)
{
    parameters_[kName] = value;
}

void AnimatorComponent::SetInt(const std::wstring& kName, int value)
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

int AnimatorComponent::GetInt(const std::wstring& kName)
{
    if (std::get_if<int>(&parameters_[kName]))
        return std::get<int>(parameters_[kName]);

    return 0;
}

std::shared_ptr<AnimatorComponent::StateNode> AnimatorComponent::GetOrAddNode(const std::wstring& kState)
{
    std::shared_ptr<StateNode> node = nullptr;

    const auto it = nodes_.find(kState);
    if (it != nodes_.end()) node = it->second;

    if (!node)
    {
        node = std::make_shared<StateNode>(kState);
        nodes_[kState] = node;
    }

    return node;
}

void AnimatorComponent::BeginPlay()
{
    ActorComponent::BeginPlay();

    std::shared_ptr<ActorComponent> component = owner_->GetComponent(SpriteRendererComponent::StaticClass());
    if (component) renderer_weak_ptr_ = std::static_pointer_cast<SpriteRendererComponent>(component);

    std::shared_ptr<SpriteRendererComponent> renderer = renderer_weak_ptr_.lock();
    if (renderer && animation_pack_ && current_animation_)
    {
        Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_pack_->target_);
        if (sprite) renderer->SetSprite(sprite, current_animation_->frames_[0]);
    }
}

void AnimatorComponent::TickComponent(float delta_time)
{
    ActorComponent::TickComponent(delta_time);

    std::shared_ptr<Transition> transition = GetTransition();
    if (transition) PlayAnimation(transition->GetTo());

    std::shared_ptr<SpriteRendererComponent> renderer_ptr = renderer_weak_ptr_.lock();
    if (!renderer_ptr || !is_playing_ || !current_animation_) return;

    const float frame_time = 1.f / current_animation_->frame_rate_;
    timer_ += delta_time;

    if (timer_ >= frame_time)
    {
        timer_ -= frame_time;
        if (current_frame_ >= current_animation_->frames_.size() - 1)
        {
            if (!current_animation_->is_loop_)
            {
                if (OnEndHandler.IsBound()) OnEndHandler.Execute();
                is_playing_ = false;
                return;
            }
        }

        current_frame_ = (current_frame_ + 1) % current_animation_->frames_.size();

        Sprite* sprite = AssetManager::Get()->Load<Sprite>(animation_pack_->target_);
        if (sprite) renderer_ptr->SetSprite(sprite, current_animation_->frames_[current_frame_]);
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
