#pragma once
#include "Actor/Component/ActorComponent.h"
#include "Misc/DelegateMacros.h"

DECLARE_DELEGATE(OnAnimationDelegate)

class Animation;
class AnimationPack;

class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

    void PlayAnimation(const std::wstring& kName);

    template<typename F, typename = std::enable_if_t<!std::is_same_v<Function<bool(void)>, std::decay_t<F>>>>
    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, F&& func);

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, M* target, bool(M::*func)(void));

    template<typename M, typename = std::enable_if_t<std::is_class_v<M>>>
    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, M* target, bool(M::*func)(void) const);

    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, bool(*func)(void));
    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo);

    void SetBool(const std::wstring& kName, bool value);
    void SetTrigger(const std::wstring& kName);

    bool GetBool(const std::wstring& kName);
    bool GetTrigger(const std::wstring& kName);

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
        Function<bool(void)> condition;
    };
    
    bool IsEnd();
    
    class SpriteRendererComponent* renderer_;
    
    AnimationPack* animation_pack_;

    Animation* current_animation_;

    float timer_;

    bool is_playing_;

    int current_frame_;

    std::unordered_map<std::wstring, std::vector<Transition>> transitions_;
    std::unordered_map<std::wstring, bool> bool_parameters_;
    std::unordered_map<std::wstring, bool> trigger_parameters_;
    
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
void AnimatorComponent::AddTransition(const std::wstring& kFrom, const std::wstring& kTo, M* target, bool(M::* func)())
{
    transitions_[kFrom].push_back({
        kTo,
        {target, func}
    });
}

template <typename M, typename>
void AnimatorComponent::AddTransition(const std::wstring& kFrom, const std::wstring& kTo, M* target,
    bool(M::* func)() const)
{
    transitions_[kFrom].push_back({
        kTo,
        {target, func}
    });
}
