#pragma once

class AnimatorComponent;
class AnimationCondition;

class AnimationTransition
{
public:
    AnimationTransition(const std::wstring& kTo, const std::shared_ptr<AnimationCondition>& kCondition);
    ~AnimationTransition() = default;

    inline const std::wstring& GetTo() const { return to_; }

    bool CheckCondition(AnimatorComponent* animator);

protected:
    std::wstring to_;
    std::shared_ptr<AnimationCondition> condition_;
    
};
