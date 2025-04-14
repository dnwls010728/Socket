#pragma once
#include "BT/Node.h"

namespace BT
{
    class WaitRange : public Node
    {
    public:
        WaitRange(const std::wstring& kName, float min, float max);
        virtual ~WaitRange() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

        static std::shared_ptr<WaitRange> Create(const std::wstring& kName, float min, float max);

    private:
        float min_;
        float max_;
        float timer_;

        bool is_started_;
    
    };
}
