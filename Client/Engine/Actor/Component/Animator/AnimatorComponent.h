#pragma once
#include <variant>

#include "Actor/Component/ActorComponent.h"
#include "Misc/DelegateMacros.h"

DECLARE_DELEGATE(OnAnimationDelegate)

class Animation;
class AnimationPack;

using ParameterType = std::variant<bool, float, int>;

class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

    void PlayAnimation(const std::wstring& kName);

    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(AnimatorComponent*)>, std::decay_t<F>>>>
    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, M* target, bool(M::*func)(AnimatorComponent*));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, M* target, bool(M::*func)(AnimatorComponent*) const);

    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, bool(*func)(AnimatorComponent*));
    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo);

    void SetBool(const std::wstring& kName, bool value);
    void SetFloat(const std::wstring& kName, float value);
    void SetInt(const std::wstring& kName, int value);
    void SetTrigger(const std::wstring& kName);

    bool GetBool(const std::wstring& kName);
    bool GetTrigger(const std::wstring& kName);
    
    float GetFloat(const std::wstring& kName);
    int GetInt(const std::wstring& kName);

    FORCEINLINE void SetAnimationPack(AnimationPack* animation_pack) { animation_pack_ = animation_pack; }

    FORCEINLINE Animation* GetCurrentAnimation() const { return current_animation_; }

    FORCEINLINE bool IsPlaying() const { return is_playing_; }

    OnAnimationDelegate OnEndHandler;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    struct Transition
    {
        std::wstring name;
        Function<bool(AnimatorComponent*)> condition;
    };
    
    bool IsEnd(AnimatorComponent* animator);
    
    std::weak_ptr<class SpriteRendererComponent> renderer_weak_ptr_;
    
    AnimationPack* animation_pack_;

    Animation* current_animation_;

    float timer_;

    bool is_playing_;

    int current_frame_;

    std::unordered_map<std::wstring, std::vector<Transition>> transitions_;
    std::unordered_map<std::wstring, ParameterType> parameters_;
    
};

template <typename F, typename>
void AnimatorComponent::AddTransition(const std::wstring& kFrom, const std::wstring& kTo, F&& func)
{
    transitions_[kFrom].push_back({
        kTo,
        std::forward<F>(func)
    });
}

template <typename M, typename>
void AnimatorComponent::AddTransition(const std::wstring& kFrom, const std::wstring& kTo, M* target, bool(M::* func)(AnimatorComponent*))
{
    transitions_[kFrom].push_back({
        kTo,
        {target, func}
    });
}

template <typename M, typename>
void AnimatorComponent::AddTransition(const std::wstring& kFrom, const std::wstring& kTo, M* target,
    bool(M::* func)(AnimatorComponent*) const)
{
    transitions_[kFrom].push_back({
        kTo,
        {target, func}
    });
}
