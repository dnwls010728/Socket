#pragma once
#include "Character/BehaviorTree/Node.h"

namespace BT
{
    class TimeOut : public Node
    {
    public:
        TimeOut(const std::wstring& kName, float duration);
        virtual ~TimeOut() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        float duration_;
        float timer_;
    
    };
}
