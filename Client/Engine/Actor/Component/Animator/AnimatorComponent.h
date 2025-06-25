#pragma once
#include <variant>

#include "Actor/Component/ActorComponent.h"
#include "Misc/DelegateMacros.h"

class Condition;
class Transition;
class Animation;
class AnimationPack;

DECLARE_DELEGATE(OnAnimationDelegate)

using ParameterType = std::variant<bool, float, int32_t>;

class AnimatorComponent : public ActorComponent
{
    SHADER_CLASS_HELPER(AnimatorComponent)
    GENERATED_BODY(AnimatorComponent, ActorComponent)

private:
    class StateNode
    {
    public:
        StateNode(const std::wstring& name);
        ~StateNode() = default;
        
        void AddTransition(const std::wstring& kTo, const std::shared_ptr<Condition>& kCondition);

        FORCEINLINE const std::wstring& GetName() const { return name_; }
        
        FORCEINLINE void SetAnimation(const std::shared_ptr<Animation>& kAnimation) { animation_ = kAnimation; }
        FORCEINLINE const std::shared_ptr<Animation>& GetAnimation() const { return animation_; }
        
        FORCEINLINE const std::unordered_set<std::shared_ptr<Transition>>& GetTransitions() const { return transitions_; }

    private:
        std::wstring name_;
        std::shared_ptr<Animation> animation_;
        std::unordered_set<std::shared_ptr<Transition>> transitions_;
    };
    
public:
    AnimatorComponent(Actor* owner, const std::wstring& kName);
    virtual ~AnimatorComponent() override = default;

    void AddTransition(const std::wstring& kFrom, const std::wstring& kTo, const std::shared_ptr<Condition>& kCondition);
    void AddAnyTransition(const std::wstring& kTo, const std::shared_ptr<Condition>& kCondition);
    void PlayAnimation(const std::wstring& kName);

    void SetBool(const std::wstring& kName, bool value);
    void SetFloat(const std::wstring& kName, float value);
    void SetInt(const std::wstring& kName, int32_t value);
    void SetTrigger(const std::wstring& kName);

    bool GetBool(const std::wstring& kName);
    bool GetTrigger(const std::wstring& kName);
    
    float GetFloat(const std::wstring& kName);
    
    int32_t GetInt(const std::wstring& kName);
    
    std::shared_ptr<StateNode> GetOrAddNode(const std::wstring& name);
    
    FORCEINLINE std::shared_ptr<StateNode> GetCurrentState() const { return current_state_; }

    FORCEINLINE void SetAnimationPack(AnimationPack* animation_pack) { animation_pack_ = animation_pack; }
    
    FORCEINLINE bool IsPlaying() const { return is_playing_; }

    OnAnimationDelegate OnEndHandler;

protected:
    virtual void InitializeComponent() override;
    virtual void BeginPlay() override;
    virtual void TickComponent(float delta_time) override;

private:
    std::shared_ptr<Transition> GetTransition();
    
    std::weak_ptr<class SpriteRendererComponent> renderer_weak_ptr_;
    
    AnimationPack* animation_pack_;

    float timer_;

    bool is_playing_;

    int32_t current_frame_;

    std::shared_ptr<StateNode> current_state_;

    std::unordered_map<std::wstring, std::shared_ptr<StateNode>> nodes_;
    std::unordered_set<std::shared_ptr<Transition>> any_transitions_;
    
    std::unordered_map<std::wstring, ParameterType> parameters_;
    
};
