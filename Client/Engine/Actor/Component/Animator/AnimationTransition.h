#pragma once

class Condition;
class AnimatorComponent;

class Transition
{
public:
    Transition(const std::wstring& kTo, const std::shared_ptr<Condition>& kCondition);
    ~Transition() = default;

    inline const std::wstring& GetTo() const { return to_; }

    bool CheckCondition(AnimatorComponent* animator);

protected:
    std::wstring to_;
    std::shared_ptr<Condition> condition_;
    
};
