#pragma once
#include "Node.h"

namespace BT
{
    class WaitRange : public Node
    {
    public:
        WaitRange(const std::wstring& kName, float min = 1.f, float max = 3.f);
        virtual ~WaitRange() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        float min_;
        float max_;
        float timer_;
    
    };
}
