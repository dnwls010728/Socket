#pragma once
#include "BT/Node.h"

namespace BT
{
    class Wait : public Node
    {
    public:
        Wait(const std::wstring& kName, float duration);
        virtual ~Wait() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

    protected:
        float duration_;
        float timer_;

        bool is_started_;
    
    };
}
