#pragma once
#include "Node.h"

namespace BT
{
    class Repeat : public Node
    {
    public:
        Repeat(const std::wstring& kName, int count);
        virtual ~Repeat() override = default;

        virtual Status TickNode(float delta_time) override;

        virtual void Reset() override;

    private:
        int count_;
        int current_count_;
    
    };
}
