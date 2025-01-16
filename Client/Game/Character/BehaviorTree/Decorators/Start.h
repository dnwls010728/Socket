#pragma once
#include "Character/BehaviorTree/Node.h"

namespace BT
{
    class Start : public Node
    {
    public:
        Start(const std::wstring& kName);
        virtual ~Start() override = default;

        virtual Status TickNode(float delta_time) override;

        FORCEINLINE void SetRepeat(bool is_repeat) { is_repeat_ = is_repeat; }
        FORCEINLINE bool IsRepeat() const { return is_repeat_; }

    private:
        bool is_repeat_;
        
    };
}
