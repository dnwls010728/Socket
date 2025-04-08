#pragma once
#include <variant>

#include "Actor/Component/ActorComponent.h"
#include "Misc/DelegateMacros.h"

class AnimationCondition;
class AnimationTransition;
class Animation;
class AnimationPack;

DECLARE_DELEGATE(OnAnimationDelegate)

using ParameterType = std::variant<bool, float, int>;

// TODO: 애니메이션 전환 구조를 변경함에 따라 테스트 필요
class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)

private:
    class StateNode
    {
    public:
        StateNode(const std::wstring& kState);
        ~StateNode() = default;

        void AddTransition(const std::wstring& kTo, const std::shared_ptr<AnimationCondition>& kCondition);

        inline const std::wstring& GetState() const { return state_; }
        inline const std::unordered_set<std::shared_ptr<AnimationTransition>>& GetTransitions() const { return transitions_; }

    private:
        std::wstring state_;
        std::unordered_set<std::shared_ptr<AnimationTransition>> transitions_;
    };
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, const std::shared_ptr<AnimationCondition>& kCondition);
    void AddAnyTransition(const std::wstring& kTo, const std::shared_ptr<AnimationCondition>& kCondition);
    void PlayAnimation(const std::wstring& kName);

    void SetBool(const std::wstring& kName, bool value);
    void SetFloat(const std::wstring& kName, float value);
    void SetInt(const std::wstring& kName, int value);
    void SetTrigger(const std::wstring& kName);

    bool GetBool(const std::wstring& kName);
    bool GetTrigger(const std::wstring& kName);
    
    float GetFloat(const std::wstring& kName);
    
    int GetInt(const std::wstring& kName);
    
    std::shared_ptr<StateNode> GetOrAddNode(const std::wstring& kState);

    FORCEINLINE void SetAnimationPack(AnimationPack* animation_pack) { animation_pack_ = animation_pack; }

    FORCEINLINE Animation* GetCurrentAnimation() const { return current_animation_; }

    FORCEINLINE bool IsPlaying() const { return is_playing_; }

    OnAnimationDelegate OnEndHandler;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    std::shared_ptr<AnimationTransition> GetTransition();
    
    std::weak_ptr<class SpriteRendererComponent> renderer_weak_ptr_;
    
    AnimationPack* animation_pack_;

    Animation* current_animation_;

    float timer_;

    bool is_playing_;

    int current_frame_;

    std::shared_ptr<StateNode> current_state_;

    std::unordered_map<std::wstring, std::shared_ptr<StateNode>> nodes_;
    std::unordered_set<std::shared_ptr<AnimationTransition>> any_transitions_;
    
    std::unordered_map<std::wstring, ParameterType> parameters_;
    
};
